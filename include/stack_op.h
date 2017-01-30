#ifndef INCLUDE_STACK_OP_H
#define INCLUDE_STACK_OP_H

#include "ast.h"

struct jaz_ast_entry *jaz_op_push(struct jaz_ast *ast, struct jaz_ast_entry *entry);
struct jaz_ast_entry *jaz_op_pop(struct jaz_ast *ast, struct jaz_ast_entry *entry);
struct jaz_ast_entry *jaz_op_rvalue(struct jaz_ast *ast, struct jaz_ast_entry *entry);
struct jaz_ast_entry *jaz_op_lvalue(struct jaz_ast *ast, struct jaz_ast_entry *entry);
struct jaz_ast_entry *jaz_op_assign(struct jaz_ast *ast, struct jaz_ast_entry *entry);
struct jaz_ast_entry *jaz_op_copy(struct jaz_ast *ast, struct jaz_ast_entry *entry);

#endif
