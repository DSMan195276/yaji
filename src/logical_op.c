
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

struct jaz_ast_entry *jaz_op_logic_and(struct jaz_ast *ast, struct jaz_ast_entry *entry)
{
    ast->stack_top--;
    *ast->stack_top = ast->stack_top[0] & ast->stack_top[1];
    return jaz_ast_next_entry(entry);
}

struct jaz_ast_entry *jaz_op_logic_or(struct jaz_ast *ast, struct jaz_ast_entry *entry)
{
    ast->stack_top--;
    *ast->stack_top = ast->stack_top[0] | ast->stack_top[1];
    return jaz_ast_next_entry(entry);
}

struct jaz_ast_entry *jaz_op_logic_not(struct jaz_ast *ast, struct jaz_ast_entry *entry)
{
    *ast->stack_top = !ast->stack_top[0];
    return jaz_ast_next_entry(entry);
}

