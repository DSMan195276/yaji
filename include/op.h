#ifndef INCLUDE_OP_H
#define INCLUDE_OP_H

#include "ast.h"

typedef struct jaz_ast_entry *(*jaz_op_callback_t) (struct jaz_ast *, struct jaz_ast_entry *);

extern jaz_op_callback_t jaz_op_lookup_table[];

#endif
