
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

struct jaz_ast_entry *jaz_op_begin(struct jaz_ast *ast, struct jaz_ast_entry *entry)
{
    struct jaz_ast_scope *scope;

    if (list_is_last(&ast->scope_list, &ast->lvalue_scope->scope_entry)) {
        scope = jaz_ast_new_scope();
        list_add_tail(&ast->scope_list, &scope->scope_entry);
    } else {
        scope = list_next_entry(ast->lvalue_scope, scope_entry);
    }

    ast->lvalue_scope = scope;
    ast->lvalue_scope_depth++;

    return jaz_ast_next_entry(entry);
}

struct jaz_ast_entry *jaz_op_end(struct jaz_ast *ast, struct jaz_ast_entry *entry)
{
    struct jaz_ast_scope *scope = ast->rvalue_scope;

    ast->rvalue_scope = list_prev_entry(scope, scope_entry);

    if (ast->rvalue_scope_depth > ast->lvalue_scope_depth) {
        list_del(&scope->scope_entry);
        jaz_ast_del_scope(scope);
    }

    ast->rvalue_scope_depth--;
    return jaz_ast_next_entry(entry);
}

struct jaz_ast_entry *jaz_op_call(struct jaz_ast *ast, struct jaz_ast_entry *entry)
{
    struct jaz_ast_scope *scope;
    struct jaz_ast_entry *nent;

    if (list_is_last(&ast->scope_list, &ast->rvalue_scope->scope_entry)) {
        scope = jaz_ast_new_scope();
        list_add_tail(&ast->scope_list, &scope->scope_entry);
    } else {
        scope = list_next_entry(ast->rvalue_scope, scope_entry);
    }

    ast->rvalue_scope = scope;
    ast->rvalue_scope_depth++;

    nent = jaz_ast_next_entry(entry);

    ast->stack_top++;
    *ast->stack_top = (intptr_t)nent;

    return entry->data.label;
}

struct jaz_ast_entry *jaz_op_return(struct jaz_ast *ast, struct jaz_ast_entry *entry)
{
    struct jaz_ast_entry *ret;
    struct jaz_ast_scope *scope = ast->lvalue_scope;

    ast->lvalue_scope = list_prev_entry(scope, scope_entry);

    if (ast->lvalue_scope_depth > ast->rvalue_scope_depth) {
        list_del(&scope->scope_entry);
        jaz_ast_del_scope(scope);
    }

    ast->lvalue_scope_depth--;

    ret = (struct jaz_ast_entry *)*ast->stack_top;
    ast->stack_top--;

    return ret;
}

