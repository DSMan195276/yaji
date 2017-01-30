#ifndef INCLUDE_HASHTABLE_H
#define INCLUDE_HASHTABLE_H

struct hlist_node {
    struct hlist_node *next;
};

typedef struct hlist_node hlist_node_t;

#define HASHTABLE_MAX 1024

struct hashtable {
    hlist_node_t *table[HASHTABLE_MAX];

    int (*cmp)  (hlist_node_t *, hlist_node_t *);
    int (*hash) (hlist_node_t *, int max);
};

hlist_node_t *hashtable_lookup(struct hashtable *, hlist_node_t *key);

void hashtable_add(struct hashtable *, hlist_node_t *);
void hashtable_clear(struct hashtable *, void (*clear) (hlist_node_t *));

#endif
