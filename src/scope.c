
#include <stdlib.h>
#include <string.h>

#include "scope.h"

int jaz_vm_variable_cmp(hlist_node_t *node1, hlist_node_t *node2)
{
    struct jaz_vm_variable *var1 = container_of(node1, struct jaz_vm_variable, node);
    struct jaz_vm_variable *var2 = container_of(node2, struct jaz_vm_variable, node);
    return strcmp(var1->id, var2->id);
}

int jaz_vm_variable_hash(hlist_node_t *node, int max)
{
    struct jaz_vm_variable *var = container_of(node, struct jaz_vm_variable, node);
    char *c;
    int hash = 0;

    for (c = var->id; *c; c++)
        hash += *c;

    return hash % max;
}

void jaz_vm_variable_free(hlist_node_t *node)
{
    struct jaz_vm_variable *var = container_of(node, struct jaz_vm_variable, node);

    free(var->id);
    free(var);
}

struct jaz_vm_scope *jaz_vm_new_scope(void)
{
    struct jaz_vm_scope *scope = malloc(sizeof(*scope));
    memset(scope, 0, sizeof(*scope));

    list_node_init(&scope->scope_entry);

    scope->variable_table.cmp = jaz_vm_variable_cmp;
    scope->variable_table.hash = jaz_vm_variable_hash;

    return scope;
}

void jaz_vm_del_scope(struct jaz_vm_scope *scope)
{
    hashtable_clear(&scope->variable_table, jaz_vm_variable_free);
    free(scope);
}

