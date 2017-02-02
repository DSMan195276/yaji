#ifndef INCLUDE_SCOPE_H
#define INCLUDE_SCOPE_H

#include <stdint.h>
#include "list.h"
#include "hashtable.h"

struct jaz_vm_variable {
    hlist_node_t node;
    char *id;
    intptr_t value;
};

struct jaz_vm_scope {
    list_node_t scope_entry;

    struct hashtable variable_table;
};

int jaz_vm_variable_cmp(hlist_node_t *node1, hlist_node_t *node2);
int jaz_vm_variable_hash(hlist_node_t *node, int max);
void jaz_vm_variable_free(hlist_node_t *node);

struct jaz_vm_scope *jaz_vm_new_scope(void);
void jaz_vm_del_scope(struct jaz_vm_scope *);

#endif
