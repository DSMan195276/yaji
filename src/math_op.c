
#include <stdio.h>
#include <stdlib.h>
#include "vm.h"

struct jaz_ast_entry *jaz_op_minus(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    vm->stack_top[-1] = vm->stack_top[-1] - vm->stack_top[0];
    vm->stack_top--;
    return list_next_entry(entry, ast_entry);
}

struct jaz_ast_entry *jaz_op_plus(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    vm->stack_top[-1] = vm->stack_top[-1] + vm->stack_top[0];
    vm->stack_top--;
    return list_next_entry(entry, ast_entry);
}

struct jaz_ast_entry *jaz_op_mult(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    vm->stack_top[-1] = vm->stack_top[-1] * vm->stack_top[0];
    vm->stack_top--;
    return list_next_entry(entry, ast_entry);
}

struct jaz_ast_entry *jaz_op_div(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    vm->stack_top[-1] = vm->stack_top[-1] / vm->stack_top[0];
    vm->stack_top--;
    return list_next_entry(entry, ast_entry);
}

struct jaz_ast_entry *jaz_op_mod(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    vm->stack_top[-1] = vm->stack_top[-1] % vm->stack_top[0];
    vm->stack_top--;
    return list_next_entry(entry, ast_entry);
}

