
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

#include "op.h"

/*
 * Subroutine operations
 */
static struct jaz_ast_entry *jaz_op_begin(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    struct jaz_vm_scope *scope;

    if (list_is_last(&vm->scope_list, &vm->lvalue_scope->scope_entry)) {
        scope = jaz_vm_new_scope();
        list_add_tail(&vm->scope_list, &scope->scope_entry);
    } else {
        scope = list_next_entry(vm->lvalue_scope, scope_entry);
    }

    vm->lvalue_scope = scope;
    vm->lvalue_scope_depth++;

    return jaz_ast_next_entry(entry);
}

static struct jaz_ast_entry *jaz_op_end(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    struct jaz_vm_scope *scope = vm->rvalue_scope;

    vm->rvalue_scope = list_prev_entry(scope, scope_entry);

    if (vm->rvalue_scope_depth > vm->lvalue_scope_depth) {
        list_del(&scope->scope_entry);
        jaz_vm_del_scope(scope);
    }

    vm->rvalue_scope_depth--;
    return jaz_ast_next_entry(entry);
}

static struct jaz_ast_entry *jaz_op_call(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    struct jaz_vm_scope *scope;
    struct jaz_ast_entry *nent;

    if (list_is_last(&vm->scope_list, &vm->rvalue_scope->scope_entry)) {
        scope = jaz_vm_new_scope();
        list_add_tail(&vm->scope_list, &scope->scope_entry);
    } else {
        scope = list_next_entry(vm->rvalue_scope, scope_entry);
    }

    vm->rvalue_scope = scope;
    vm->rvalue_scope_depth++;

    nent = jaz_ast_next_entry(entry);

    vm->stack_top++;
    *vm->stack_top = (intptr_t)nent;

    return entry->data.label;
}

static struct jaz_ast_entry *jaz_op_return(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    struct jaz_ast_entry *ret;
    struct jaz_vm_scope *scope = vm->lvalue_scope;

    vm->lvalue_scope = list_prev_entry(scope, scope_entry);

    if (vm->lvalue_scope_depth > vm->rvalue_scope_depth) {
        list_del(&scope->scope_entry);
        jaz_vm_del_scope(scope);
    }

    vm->lvalue_scope_depth--;

    ret = (struct jaz_ast_entry *)*vm->stack_top;
    vm->stack_top--;

    return ret;
}

/*
 * Stack manipulation operations
 */
static struct jaz_ast_entry *jaz_op_push(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    vm->stack_top++;
    *vm->stack_top = atoll(entry->data.param);
    return list_next_entry(entry, ast_entry);
}

static struct jaz_ast_entry *jaz_op_pop(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    vm->stack_top--;
    return list_next_entry(entry, ast_entry);
}

static struct jaz_ast_entry *jaz_op_rvalue(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    hlist_node_t *ent;
    struct hashtable *table = &vm->rvalue_scope->variable_table;
    struct jaz_vm_variable *var;
    struct jaz_vm_variable find = { .id = entry->data.param };
    intptr_t val = 0;

    ent = hashtable_lookup(table, &find.node);

    if (ent) {
        var = container_of(ent, struct jaz_vm_variable, node);
        val = var->value;
    }

    vm->stack_top++;
    *vm->stack_top = val;

    return list_next_entry(entry, ast_entry);
}

static struct jaz_ast_entry *jaz_op_lvalue(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    hlist_node_t *ent;
    struct hashtable *table = &vm->lvalue_scope->variable_table;
    struct jaz_vm_variable *var;
    struct jaz_vm_variable find = { .id = entry->data.param };

    ent = hashtable_lookup(table, &find.node);

    if (!ent) {
        var = malloc(sizeof(*var));
        memset(var, 0, sizeof(*var));

        var->id = strdup(entry->data.param);

        hashtable_add(table, &var->node);
    } else {
        var = container_of(ent, struct jaz_vm_variable, node);
    }

    vm->stack_top++;
    *vm->stack_top = (intptr_t)var;

    return list_next_entry(entry, ast_entry);
}

static struct jaz_ast_entry *jaz_op_assign(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    struct jaz_vm_variable *var = (struct jaz_vm_variable *)vm->stack_top[-1];
    intptr_t val = vm->stack_top[0];

    var->value = val;

    vm->stack_top -= 2;

    return list_next_entry(entry, ast_entry);
}

static struct jaz_ast_entry *jaz_op_copy(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    vm->stack_top++;
    *vm->stack_top = vm->stack_top[-1];
    return list_next_entry(entry, ast_entry);
}

/*
 * Math operations
 */
static struct jaz_ast_entry *jaz_op_minus(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    vm->stack_top[-1] = vm->stack_top[-1] - vm->stack_top[0];
    vm->stack_top--;
    return list_next_entry(entry, ast_entry);
}

static struct jaz_ast_entry *jaz_op_plus(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    vm->stack_top[-1] = vm->stack_top[-1] + vm->stack_top[0];
    vm->stack_top--;
    return list_next_entry(entry, ast_entry);
}

static struct jaz_ast_entry *jaz_op_mult(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    vm->stack_top[-1] = vm->stack_top[-1] * vm->stack_top[0];
    vm->stack_top--;
    return list_next_entry(entry, ast_entry);
}

static struct jaz_ast_entry *jaz_op_div(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    vm->stack_top[-1] = vm->stack_top[-1] / vm->stack_top[0];
    vm->stack_top--;
    return list_next_entry(entry, ast_entry);
}

static struct jaz_ast_entry *jaz_op_mod(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    vm->stack_top[-1] = vm->stack_top[-1] % vm->stack_top[0];
    vm->stack_top--;
    return list_next_entry(entry, ast_entry);
}

/*
 * Logical operations
 */
static struct jaz_ast_entry *jaz_op_logic_and(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    vm->stack_top--;
    *vm->stack_top = vm->stack_top[0] & vm->stack_top[1];
    return jaz_ast_next_entry(entry);
}

static struct jaz_ast_entry *jaz_op_logic_or(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    vm->stack_top--;
    *vm->stack_top = vm->stack_top[0] | vm->stack_top[1];
    return jaz_ast_next_entry(entry);
}

static struct jaz_ast_entry *jaz_op_logic_not(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    *vm->stack_top = !vm->stack_top[0];
    return jaz_ast_next_entry(entry);
}

/*
 * Control-flow operations
 */
static struct jaz_ast_entry *jaz_op_goto(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    return entry->data.label;
}

static struct jaz_ast_entry *jaz_op_gofalse(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    int b = *vm->stack_top;
    vm->stack_top--;

    if (!b)
        return entry->data.label;
    else
        return jaz_ast_next_entry(entry);
}

static struct jaz_ast_entry *jaz_op_gotrue(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    int b = *vm->stack_top;
    vm->stack_top--;

    if (b)
        return entry->data.label;
    else
        return jaz_ast_next_entry(entry);
}

static struct jaz_ast_entry *jaz_op_halt(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    vm->halt = 1;
    return jaz_ast_next_entry(entry);
}

static struct jaz_ast_entry *jaz_op_label(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    return jaz_ast_next_entry(entry);
}

/*
 * Printing operations
 */
static struct jaz_ast_entry *jaz_op_show(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    printf("%s\n", entry->data.param);
    return list_next_entry(entry, ast_entry);
}

static struct jaz_ast_entry *jaz_op_print(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    printf("%" PRIdPTR "\n", *vm->stack_top);
    return list_next_entry(entry, ast_entry);
}

/*
 * Comparision operations
 */
static struct jaz_ast_entry *jaz_op_not_equal(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    vm->stack_top--;
    *vm->stack_top = vm->stack_top[0] != vm->stack_top[1];
    return jaz_ast_next_entry(entry);
}

static struct jaz_ast_entry *jaz_op_equal(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    vm->stack_top--;
    *vm->stack_top = vm->stack_top[0] == vm->stack_top[1];
    return jaz_ast_next_entry(entry);
}

static struct jaz_ast_entry *jaz_op_less_than(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    vm->stack_top--;
    *vm->stack_top = vm->stack_top[0] < vm->stack_top[1];
    return jaz_ast_next_entry(entry);
}

static struct jaz_ast_entry *jaz_op_less_than_equal(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    vm->stack_top--;
    *vm->stack_top = vm->stack_top[0] <= vm->stack_top[1];
    return jaz_ast_next_entry(entry);
}

static struct jaz_ast_entry *jaz_op_greater_than(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    vm->stack_top--;
    *vm->stack_top = vm->stack_top[0] > vm->stack_top[1];
    return jaz_ast_next_entry(entry);
}

static struct jaz_ast_entry *jaz_op_greater_than_equal(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    vm->stack_top--;
    *vm->stack_top = vm->stack_top[0] >= vm->stack_top[1];
    return jaz_ast_next_entry(entry);
}

jaz_op_callback_t jaz_op_lookup_table[] = {
    ['='] = jaz_op_equal,
    ['<'] = jaz_op_less_than,
    ['>'] = jaz_op_greater_than,
    [TOK_NOT_EQUAL] = jaz_op_not_equal,
    [TOK_LESS_THAN_EQUAL] = jaz_op_less_than_equal,
    [TOK_GREATER_THAN_EQUAL] = jaz_op_greater_than_equal,

    [TOK_SHOW] = jaz_op_show,
    [TOK_PRINT] = jaz_op_print,

    [TOK_GOTO] = jaz_op_goto,
    [TOK_GOFALSE] = jaz_op_gofalse,
    [TOK_GOTRUE] = jaz_op_gotrue,
    [TOK_HALT] = jaz_op_halt,
    [TOK_LABEL] = jaz_op_label,

    ['&'] = jaz_op_logic_and,
    ['|'] = jaz_op_logic_or,
    ['!'] = jaz_op_logic_not,

    ['-'] = jaz_op_minus,
    ['+'] = jaz_op_plus,
    ['*'] = jaz_op_mult,
    ['/'] = jaz_op_div,
    [TOK_MOD] = jaz_op_mod,

    [TOK_PUSH] = jaz_op_push,
    [TOK_POP] = jaz_op_pop,
    [TOK_RVALUE] = jaz_op_rvalue,
    [TOK_LVALUE] = jaz_op_lvalue,
    [TOK_ASSIGN] = jaz_op_assign,
    [TOK_COPY] = jaz_op_copy,

    [TOK_BEGIN] = jaz_op_begin,
    [TOK_END] = jaz_op_end,
    [TOK_RETURN] = jaz_op_return,
    [TOK_CALL] = jaz_op_call,
};

