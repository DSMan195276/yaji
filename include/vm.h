#ifndef INCLUDE_VM_H
#define INCLUDE_VM_H

#include "list.h"
#include "scope.h"
#include "ast.h"

#define STACK_MAX 1000

struct jaz_vm {
    struct jaz_ast ast;

    intptr_t call_stack[STACK_MAX];
    intptr_t *stack_top;

    list_head_t scope_list;

    int lvalue_scope_depth, rvalue_scope_depth;
    struct jaz_vm_scope *lvalue_scope;
    struct jaz_vm_scope *rvalue_scope;

    unsigned int halt :1;
};

#define JAZ_VM(vm)\
    { \
        .ast = JAZ_AST((vm).ast), \
        .scope_list = LIST_HEAD_INIT((vm).scope_list), \
        .stack_top = (vm).call_stack, \
    }

static inline void jaz_vm_init(struct jaz_vm *vm)
{
    *vm = (struct jaz_vm)JAZ_VM(*vm);
}

int jaz_vm_load(struct jaz_vm *vm, const char *file);
void jaz_vm_run(struct jaz_vm *);
void jaz_vm_clear(struct jaz_vm *);

#endif
