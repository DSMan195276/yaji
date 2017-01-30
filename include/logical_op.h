#ifndef INCLUDE_LOGICAL_OP_H
#define INCLUDE_LOGICAL_OP_H

#include "ast.h"

struct jaz_ast_entry *jaz_op_logic_and(struct jaz_ast *ast, struct jaz_ast_entry *entry);
struct jaz_ast_entry *jaz_op_logic_or(struct jaz_ast *ast, struct jaz_ast_entry *entry);
struct jaz_ast_entry *jaz_op_logic_not(struct jaz_ast *ast, struct jaz_ast_entry *entry);

#endif
