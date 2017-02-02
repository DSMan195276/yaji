
#include <stdio.h>
#include <stdlib.h>
#include "vm.h"

struct jaz_ast_entry *jaz_op_logic_and(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    vm->stack_top--;
    *vm->stack_top = vm->stack_top[0] & vm->stack_top[1];
    return jaz_ast_next_entry(entry);
}

struct jaz_ast_entry *jaz_op_logic_or(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    vm->stack_top--;
    *vm->stack_top = vm->stack_top[0] | vm->stack_top[1];
    return jaz_ast_next_entry(entry);
}

struct jaz_ast_entry *jaz_op_logic_not(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    *vm->stack_top = !vm->stack_top[0];
    return jaz_ast_next_entry(entry);
}

