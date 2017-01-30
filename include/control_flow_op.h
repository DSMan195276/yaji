#ifndef INCLUDE_CONTROL_FLOW_OP_H
#define INCLUDE_CONTROL_FLOW_OP_H

#include "ast.h"

struct jaz_ast_entry *jaz_op_goto(struct jaz_ast *ast, struct jaz_ast_entry *entry);
struct jaz_ast_entry *jaz_op_gofalse(struct jaz_ast *ast, struct jaz_ast_entry *entry);
struct jaz_ast_entry *jaz_op_gotrue(struct jaz_ast *ast, struct jaz_ast_entry *entry);
struct jaz_ast_entry *jaz_op_halt(struct jaz_ast *ast, struct jaz_ast_entry *entry);
struct jaz_ast_entry *jaz_op_label(struct jaz_ast *ast, struct jaz_ast_entry *entry);

#endif
