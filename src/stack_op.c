
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"

struct jaz_ast_entry *jaz_op_push(struct jaz_ast *ast, struct jaz_ast_entry *entry)
{
    ast->stack_top++;
    *ast->stack_top = atoll(entry->data.param);
    return list_next_entry(entry, ast_entry);
}

struct jaz_ast_entry *jaz_op_pop(struct jaz_ast *ast, struct jaz_ast_entry *entry)
{
    ast->stack_top--;
    return list_next_entry(entry, ast_entry);
}

struct jaz_ast_entry *jaz_op_rvalue(struct jaz_ast *ast, struct jaz_ast_entry *entry)
{
    hlist_node_t *ent;
    struct hashtable *table = &ast->rvalue_scope->variable_table;
    struct jaz_ast_variable *var;
    struct jaz_ast_variable find = { .id = entry->data.param };
    intptr_t val = 0;

    ent = hashtable_lookup(table, &find.node);

    if (ent) {
        var = container_of(ent, struct jaz_ast_variable, node);
        val = var->value;
    }

    ast->stack_top++;
    *ast->stack_top = val;

    return list_next_entry(entry, ast_entry);
}

struct jaz_ast_entry *jaz_op_lvalue(struct jaz_ast *ast, struct jaz_ast_entry *entry)
{
    hlist_node_t *ent;
    struct hashtable *table = &ast->lvalue_scope->variable_table;
    struct jaz_ast_variable *var;
    struct jaz_ast_variable find = { .id = entry->data.param };

    ent = hashtable_lookup(table, &find.node);

    if (!ent) {
        var = malloc(sizeof(*var));
        memset(var, 0, sizeof(*var));

        var->id = strdup(entry->data.param);

        hashtable_add(table, &var->node);
    } else {
        var = container_of(ent, struct jaz_ast_variable, node);
    }

    ast->stack_top++;
    *ast->stack_top = (intptr_t)var;

    return list_next_entry(entry, ast_entry);
}

struct jaz_ast_entry *jaz_op_assign(struct jaz_ast *ast, struct jaz_ast_entry *entry)
{
    struct jaz_ast_variable *var = (struct jaz_ast_variable *)ast->stack_top[-1];
    intptr_t val = ast->stack_top[0];

    var->value = val;

    ast->stack_top -= 2;

    return list_next_entry(entry, ast_entry);
}

struct jaz_ast_entry *jaz_op_copy(struct jaz_ast *ast, struct jaz_ast_entry *entry)
{
    ast->stack_top++;
    *ast->stack_top = ast->stack_top[-1];
    return list_next_entry(entry, ast_entry);
}

