#ifndef INCLUDE_STACK_OP_H
#define INCLUDE_STACK_OP_H

#include "vm.h"

struct jaz_ast_entry *jaz_op_push(struct jaz_vm *vm, struct jaz_ast_entry *entry);
struct jaz_ast_entry *jaz_op_pop(struct jaz_vm *vm, struct jaz_ast_entry *entry);
struct jaz_ast_entry *jaz_op_rvalue(struct jaz_vm *vm, struct jaz_ast_entry *entry);
struct jaz_ast_entry *jaz_op_lvalue(struct jaz_vm *vm, struct jaz_ast_entry *entry);
struct jaz_ast_entry *jaz_op_assign(struct jaz_vm *vm, struct jaz_ast_entry *entry);
struct jaz_ast_entry *jaz_op_copy(struct jaz_vm *vm, struct jaz_ast_entry *entry);

#endif
