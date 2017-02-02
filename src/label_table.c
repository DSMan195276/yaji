
#include <string.h>
#include "label_table.h"

int jaz_label_cmp(hlist_node_t *node1, hlist_node_t *node2)
{
    struct jaz_label *label1 = container_of(node1, struct jaz_label, node);
    struct jaz_label *label2 = container_of(node2, struct jaz_label, node);
    return strcmp(label1->id, label2->id);
}

int jaz_label_hash(hlist_node_t *node, int max)
{
    struct jaz_label *label = container_of(node, struct jaz_label, node);
    char *c;
    int hash = 0;

    for (c = label->id; *c; c++)
        hash += *c;

    return hash % max;
}

void jaz_label_free(hlist_node_t *node)
{
    struct jaz_label *label = container_of(node, struct jaz_label, node);
    free(label);
}

