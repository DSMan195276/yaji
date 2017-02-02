
#include <stdio.h>
#include <stdlib.h>
#include "vm.h"

struct jaz_ast_entry *jaz_op_begin(struct jaz_vm *vm, struct jaz_ast_entry *entry)
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

struct jaz_ast_entry *jaz_op_end(struct jaz_vm *vm, struct jaz_ast_entry *entry)
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

struct jaz_ast_entry *jaz_op_call(struct jaz_vm *vm, struct jaz_ast_entry *entry)
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

struct jaz_ast_entry *jaz_op_return(struct jaz_vm *vm, struct jaz_ast_entry *entry)
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

