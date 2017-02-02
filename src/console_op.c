
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include "vm.h"

struct jaz_ast_entry *jaz_op_show(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    printf("%s\n", entry->data.param);
    return list_next_entry(entry, ast_entry);
}

struct jaz_ast_entry *jaz_op_print(struct jaz_vm *vm, struct jaz_ast_entry *entry)
{
    printf("%" PRIdPTR "\n", *vm->stack_top);
    return list_next_entry(entry, ast_entry);
}

