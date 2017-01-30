
#include <string.h>
#include <stdio.h>

#include "hashtable.h"
#include "ast.h"

hlist_node_t *hashtable_lookup(struct hashtable *table, hlist_node_t *key)
{
    int hash = (table->hash) (key, HASHTABLE_MAX);
    hlist_node_t *list = table->table[hash];

    for (; list; list = list->next) {
        if (((table->cmp) (list, key)) == 0) {
            struct jaz_ast_variable *var = container_of(list, struct jaz_ast_variable, node);
            return list;
        }
    }

    return NULL;
}

void hashtable_add(struct hashtable *table, hlist_node_t *node)
{
    int hash = (table->hash) (node, HASHTABLE_MAX);

    node->next = table->table[hash];
    table->table[hash] = node;
}

void hashtable_clear(struct hashtable *table, void (*clear) (hlist_node_t *))
{
    int i;

    for (i = 0; i < HASHTABLE_MAX; i++) {
        hlist_node_t *list = table->table[i];
        hlist_node_t *next;

        for (; list; list = next) {
            next = list->next;
            (clear) (list);
        }
    }
}
