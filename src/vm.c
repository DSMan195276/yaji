
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "op.h"
#include "vm.h"

void jaz_vm_clear(struct jaz_vm *vm)
{
    jaz_ast_clear(&vm->ast);
}

int jaz_vm_load(struct jaz_vm *vm, const char *file)
{
    return jaz_ast_create(&vm->ast, file);
}

void jaz_vm_run(struct jaz_vm *vm)
{
    struct jaz_ast_entry *entry = list_first_entry(&vm->ast.ast_list, struct jaz_ast_entry, ast_entry);
    struct jaz_vm_scope *initial_scope;

    vm->halt = 0;

    initial_scope = jaz_vm_new_scope();

    vm->lvalue_scope_depth = vm->rvalue_scope_depth = 0;
    vm->lvalue_scope = vm->rvalue_scope = initial_scope;

    list_add_tail(&vm->scope_list, &vm->lvalue_scope->scope_entry);

    while (!vm->halt && !list_ptr_is_head(&vm->ast.ast_list, &entry->ast_entry))
        entry = (jaz_op_lookup_table[entry->op]) (vm, entry);

    list_del(&initial_scope->scope_entry);
    jaz_vm_del_scope(initial_scope);

    return ;
}

