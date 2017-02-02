#ifndef INCLUDE_LABEL_TABLE_H
#define INCLUDE_LABEL_TABLE_H

#include <stdlib.h>
#include "container_of.h"
#include "hashtable.h"

struct jaz_ast_entry;

struct jaz_label {
    hlist_node_t node;
    char *id;
    struct jaz_ast_entry *location;
};

struct jaz_label_table {
    struct hashtable table;
};

int jaz_label_cmp(hlist_node_t *node1, hlist_node_t *node2);
int jaz_label_hash(hlist_node_t *node, int max);
void jaz_label_free(hlist_node_t *node);


#define JAZ_LABEL_TABLE_INIT(tab) \
    { \
        .table.cmp = jaz_label_cmp, \
        .table.hash = jaz_label_hash, \
    }

static inline void jaz_label_table_init(struct jaz_label_table *table)
{
    *table = (struct jaz_label_table)JAZ_LABEL_TABLE_INIT(*table);
}

static inline void jaz_label_table_clear(struct jaz_label_table *table)
{
    hashtable_clear(&table->table, jaz_label_free);
}

static inline void jaz_label_table_add(struct jaz_label_table *table, struct jaz_label *label)
{
    hashtable_add(&table->table, &label->node);
}

static inline struct jaz_label *jaz_label_table_find(struct jaz_label_table *table, const char *id)
{
    struct jaz_label label = { .id = (char *)id };
    hlist_node_t *find = hashtable_lookup(&table->table, &label.node);

    if (find)
        return container_of(find, struct jaz_label, node);
    else
        return NULL;
}

#endif
