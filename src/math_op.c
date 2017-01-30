
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

struct jaz_ast_entry *jaz_op_minus(struct jaz_ast *ast, struct jaz_ast_entry *entry)
{
    ast->stack_top[-1] = ast->stack_top[-1] - ast->stack_top[0];
    ast->stack_top--;
    return list_next_entry(entry, ast_entry);
}

struct jaz_ast_entry *jaz_op_plus(struct jaz_ast *ast, struct jaz_ast_entry *entry)
{
    ast->stack_top[-1] = ast->stack_top[-1] + ast->stack_top[0];
    ast->stack_top--;
    return list_next_entry(entry, ast_entry);
}

struct jaz_ast_entry *jaz_op_mult(struct jaz_ast *ast, struct jaz_ast_entry *entry)
{
    ast->stack_top[-1] = ast->stack_top[-1] * ast->stack_top[0];
    ast->stack_top--;
    return list_next_entry(entry, ast_entry);
}

struct jaz_ast_entry *jaz_op_div(struct jaz_ast *ast, struct jaz_ast_entry *entry)
{
    ast->stack_top[-1] = ast->stack_top[-1] / ast->stack_top[0];
    ast->stack_top--;
    return list_next_entry(entry, ast_entry);
}

struct jaz_ast_entry *jaz_op_mod(struct jaz_ast *ast, struct jaz_ast_entry *entry)
{
    ast->stack_top[-1] = ast->stack_top[-1] % ast->stack_top[0];
    ast->stack_top--;
    return list_next_entry(entry, ast_entry);
}

