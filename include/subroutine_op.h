#ifndef INCLUDE_SUBROUTINE_OP_H
#define INCLUDE_SUBROUTINE_OP_H

#include "ast.h"

struct jaz_ast_entry *jaz_op_begin(struct jaz_ast *ast, struct jaz_ast_entry *entry);
struct jaz_ast_entry *jaz_op_end(struct jaz_ast *ast, struct jaz_ast_entry *entry);
struct jaz_ast_entry *jaz_op_return(struct jaz_ast *ast, struct jaz_ast_entry *entry);
struct jaz_ast_entry *jaz_op_call(struct jaz_ast *ast, struct jaz_ast_entry *entry);

#endif
