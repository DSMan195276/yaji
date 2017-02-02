#ifndef INCLUDE_COMPARE_OP_H
#define INCLUDE_COMPARE_OP_H

#include "vm.h"

struct jaz_ast_entry *jaz_op_not_equal(struct jaz_vm *vm, struct jaz_ast_entry *entry);
struct jaz_ast_entry *jaz_op_equal(struct jaz_vm *vm, struct jaz_ast_entry *entry);
struct jaz_ast_entry *jaz_op_less_than(struct jaz_vm *vm, struct jaz_ast_entry *entry);
struct jaz_ast_entry *jaz_op_less_than_equal(struct jaz_vm *vm, struct jaz_ast_entry *entry);
struct jaz_ast_entry *jaz_op_greater_than(struct jaz_vm *vm, struct jaz_ast_entry *entry);
struct jaz_ast_entry *jaz_op_greater_than_equal(struct jaz_vm *vm, struct jaz_ast_entry *entry);

#endif
