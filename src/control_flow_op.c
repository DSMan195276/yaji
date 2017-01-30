
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

struct jaz_ast_entry *jaz_op_goto(struct jaz_ast *ast, struct jaz_ast_entry *entry)
{
    return entry->data.label;
}

struct jaz_ast_entry *jaz_op_gofalse(struct jaz_ast *ast, struct jaz_ast_entry *entry)
{
    int b = *ast->stack_top;
    ast->stack_top--;

    if (!b)
        return entry->data.label;
    else
        return jaz_ast_next_entry(entry);
}

struct jaz_ast_entry *jaz_op_gotrue(struct jaz_ast *ast, struct jaz_ast_entry *entry)
{
    int b = *ast->stack_top;
    ast->stack_top--;

    if (b)
        return entry->data.label;
    else
        return jaz_ast_next_entry(entry);
}

struct jaz_ast_entry *jaz_op_halt(struct jaz_ast *ast, struct jaz_ast_entry *entry)
{
    ast->halt = 1;
    return jaz_ast_next_entry(entry);
}

struct jaz_ast_entry *jaz_op_label(struct jaz_ast *ast, struct jaz_ast_entry *entry)
{
    return jaz_ast_next_entry(entry);
}

