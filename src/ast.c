
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "ast.h"
#include "op.h"

void jaz_ast_clear(struct jaz_ast *ast)
{
    struct jaz_ast_entry *entry;

    list_foreach_take_entry(&ast->ast_list, entry, ast_entry) {
        free(entry->data.param);
        free(entry);
    }

    jaz_label_table_clear(&ast->label_table);
}

static int jaz_ast_resolve_labels(struct jaz_ast *ast)
{
    struct jaz_ast_entry *entry;

    list_foreach_entry(&ast->ast_list, entry, ast_entry) {
        struct jaz_label *label;

        if (entry->op != TOK_GOTO && entry->op != TOK_GOTRUE && entry->op != TOK_GOFALSE && entry->op != TOK_CALL)
            continue;

        label = jaz_label_table_find(&ast->label_table, entry->data.param);

        if (!label) {
            printf("Error: Unknown label: \"%s\"\n", entry->data.param);
            return 1;
        }

        entry->data.label = label->location;
    }

    return 0;
}

int jaz_ast_create(struct jaz_ast *ast, const char *file)
{
    FILE *filp = fopen(file, "r");
    int ret;

    yyin = filp;
    ret = yyparse(ast);

    jaz_ast_resolve_labels(ast);

    fclose(filp);
    return ret;
}

