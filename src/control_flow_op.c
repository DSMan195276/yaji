
#include <stdio.h>
#include <stdlib.h>
#include "vm.h"

struct jaz_ast_entry *jaz_op_goto(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    return entry->data.label;
}

struct jaz_ast_entry *jaz_op_gofalse(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    int b = *vm->stack_top;
    vm->stack_top--;

    if (!b)
        return entry->data.label;
    else
        return jaz_ast_next_entry(entry);
}

struct jaz_ast_entry *jaz_op_gotrue(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    int b = *vm->stack_top;
    vm->stack_top--;

    if (b)
        return entry->data.label;
    else
        return jaz_ast_next_entry(entry);
}

struct jaz_ast_entry *jaz_op_halt(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    vm->halt = 1;
    return jaz_ast_next_entry(entry);
}

struct jaz_ast_entry *jaz_op_label(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    return jaz_ast_next_entry(entry);
}

