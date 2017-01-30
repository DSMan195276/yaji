
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

struct jaz_ast_entry *jaz_op_not_equal(struct jaz_ast *ast, struct jaz_ast_entry *entry)
{
    ast->stack_top--;
    *ast->stack_top = ast->stack_top[0] != ast->stack_top[1];
    return jaz_ast_next_entry(entry);
}

struct jaz_ast_entry *jaz_op_equal(struct jaz_ast *ast, struct jaz_ast_entry *entry)
{
    ast->stack_top--;
    *ast->stack_top = ast->stack_top[0] == ast->stack_top[1];
    return jaz_ast_next_entry(entry);
}

struct jaz_ast_entry *jaz_op_less_than(struct jaz_ast *ast, struct jaz_ast_entry *entry)
{
    ast->stack_top--;
    *ast->stack_top = ast->stack_top[0] < ast->stack_top[1];
    return jaz_ast_next_entry(entry);
}

struct jaz_ast_entry *jaz_op_less_than_equal(struct jaz_ast *ast, struct jaz_ast_entry *entry)
{
    ast->stack_top--;
    *ast->stack_top = ast->stack_top[0] <= ast->stack_top[1];
    return jaz_ast_next_entry(entry);
}

struct jaz_ast_entry *jaz_op_greater_than(struct jaz_ast *ast, struct jaz_ast_entry *entry)
{
    ast->stack_top--;
    *ast->stack_top = ast->stack_top[0] > ast->stack_top[1];
    return jaz_ast_next_entry(entry);
}

struct jaz_ast_entry *jaz_op_greater_than_equal(struct jaz_ast *ast, struct jaz_ast_entry *entry)
{
    ast->stack_top--;
    *ast->stack_top = ast->stack_top[0] >= ast->stack_top[1];
    return jaz_ast_next_entry(entry);
}

