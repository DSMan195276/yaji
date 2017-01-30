#ifndef INCLUDE_MATH_OP_H
#define INCLUDE_MATH_OP_H

#include "ast.h"

struct jaz_ast_entry *jaz_op_minus(struct jaz_ast *ast, struct jaz_ast_entry *entry);
struct jaz_ast_entry *jaz_op_plus(struct jaz_ast *ast, struct jaz_ast_entry *entry);
struct jaz_ast_entry *jaz_op_mult(struct jaz_ast *ast, struct jaz_ast_entry *entry);
struct jaz_ast_entry *jaz_op_div(struct jaz_ast *ast, struct jaz_ast_entry *entry);
struct jaz_ast_entry *jaz_op_mod(struct jaz_ast *ast, struct jaz_ast_entry *entry);

#endif
