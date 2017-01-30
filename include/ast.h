#ifndef INCLUDE_AST_H
#define INCLUDE_AST_H

#include <stdint.h>
#include "list.h"
#include "hashtable.h"

struct jaz_ast_entry_data {
    char *param;
    struct jaz_ast_entry *label;
};

struct jaz_ast_entry {
    list_node_t ast_entry;

    int op; /* TOK_* representing the operation */
    struct jaz_ast_entry_data data;
};

struct jaz_ast_label_entry {
    list_node_t ast_label_entry;
    char *id;
    struct jaz_ast_entry *location;
};

struct jaz_ast_variable {
    hlist_node_t node;
    char *id;
    intptr_t value;
};

struct jaz_ast_scope {
    list_node_t scope_entry;

    struct hashtable variable_table;
};

#define STACK_MAX 1000

struct jaz_ast {
    list_head_t ast_list;
    list_head_t ast_label_list;

    intptr_t call_stack[STACK_MAX];
    intptr_t *stack_top;

    list_head_t scope_list;

    int lvalue_scope_depth, rvalue_scope_depth;
    struct jaz_ast_scope *lvalue_scope;
    struct jaz_ast_scope *rvalue_scope;

    unsigned int halt :1;
};

#define JAZ_AST_ENTRY_INIT(ent) \
    { \
        .ast_entry = LIST_NODE_INIT((ent).ast_entry), \
    }

static inline void jaz_ast_entry_init(struct jaz_ast_entry *ent)
{
    *ent = (struct jaz_ast_entry)JAZ_AST_ENTRY_INIT(*ent);
}

#define JAZ_AST_LABEL_ENTRY_INIT(ent) \
    { \
        .ast_label_entry = LIST_NODE_INIT((ent).ast_label_entry), \
    }

static inline void jaz_ast_label_entry_init(struct jaz_ast_label_entry *ent)
{
    *ent = (struct jaz_ast_label_entry)JAZ_AST_LABEL_ENTRY_INIT(*ent);
}

#define jaz_ast_next_entry(ent) \
    list_next_entry(ent, ast_entry)

int jaz_ast_variable_cmp(hlist_node_t *node1, hlist_node_t *node2);
int jaz_ast_variable_hash(hlist_node_t *node, int max);
void jaz_ast_variable_free(hlist_node_t *node);

#define JAZ_AST(ast)\
    { \
        .ast_list = LIST_HEAD_INIT((ast).ast_list), \
        .ast_label_list = LIST_HEAD_INIT((ast).ast_label_list), \
        .scope_list = LIST_HEAD_INIT((ast).scope_list), \
        .stack_top = (ast).call_stack, \
    }

static inline void jaz_ast_init(struct jaz_ast *ast)
{
    *ast = (struct jaz_ast)JAZ_AST(*ast);
}


void jaz_ast_clear(struct jaz_ast *);
int  jaz_ast_create(struct jaz_ast *ast, const char *file);

int  jaz_ast_resolve_labels(struct jaz_ast *);
void jaz_ast_print(struct jaz_ast *);
void jaz_ast_run(struct jaz_ast *);

struct jaz_ast_scope *jaz_ast_new_scope(void);
void jaz_ast_del_scope(struct jaz_ast_scope *);

int yyparse(struct jaz_ast *);

#ifndef IN_PARSER
# include "parser.tab.h"
#endif

#endif
