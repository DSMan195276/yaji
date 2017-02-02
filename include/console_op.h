#ifndef INCLUDE_CONSOLE_OP_H
#define INCLUDE_CONSOLE_OP_H

#include "vm.h"

struct jaz_ast_entry *jaz_op_show(struct jaz_vm *vm, struct jaz_ast_entry *entry);
struct jaz_ast_entry *jaz_op_print(struct jaz_vm *vm, struct jaz_ast_entry *entry);

#endif
