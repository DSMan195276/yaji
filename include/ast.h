#ifndef INCLUDE_AST_H
#define INCLUDE_AST_H

#include <stdint.h>
#include "list.h"
#include "label_table.h"

struct jaz_ast_entry_data {
    char *param;
    struct jaz_ast_entry *label;
};

struct jaz_ast_entry {
    list_node_t ast_entry;

    int op; /* TOK_* representing the operation */
    struct jaz_ast_entry_data data;
};

struct jaz_ast {
    list_head_t ast_list;
    struct jaz_label_table label_table;
};

#define JAZ_AST_ENTRY_INIT(ent) \
    { \
        .ast_entry = LIST_NODE_INIT((ent).ast_entry), \
    }

static inline void jaz_ast_entry_init(struct jaz_ast_entry *ent)
{
    *ent = (struct jaz_ast_entry)JAZ_AST_ENTRY_INIT(*ent);
}

#define jaz_ast_next_entry(ent) \
    list_next_entry(ent, ast_entry)

#define JAZ_AST(ast) \
    { \
        .ast_list = LIST_HEAD_INIT((ast).ast_list), \
        .label_table = JAZ_LABEL_TABLE_INIT((ast).label_table), \
    }

static inline void jaz_ast_init(struct jaz_ast *ast)
{
    *ast = (struct jaz_ast)JAZ_AST(*ast);
}

void jaz_ast_clear(struct jaz_ast *);
int  jaz_ast_create(struct jaz_ast *ast, const char *file);

int yyparse(struct jaz_ast *);

#ifndef IN_PARSER
# include "parser.tab.h"
#endif

#endif
