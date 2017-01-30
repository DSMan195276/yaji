
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "ast.h"
#include "op.h"

int jaz_ast_variable_cmp(hlist_node_t *node1, hlist_node_t *node2)
{
    struct jaz_ast_variable *var1 = container_of(node1, struct jaz_ast_variable, node);
    struct jaz_ast_variable *var2 = container_of(node2, struct jaz_ast_variable, node);
    return strcmp(var1->id, var2->id);
}

int jaz_ast_variable_hash(hlist_node_t *node, int max)
{
    struct jaz_ast_variable *var = container_of(node, struct jaz_ast_variable, node);
    char *c;
    int hash = 0;

    for (c = var->id; *c; c++)
        hash += *c;

    return hash % max;
}

void jaz_ast_variable_free(hlist_node_t *node)
{
    struct jaz_ast_variable *var = container_of(node, struct jaz_ast_variable, node);

    free(var->id);
    free(var);
}

struct jaz_ast_scope *jaz_ast_new_scope(void)
{
    struct jaz_ast_scope *scope = malloc(sizeof(*scope));
    memset(scope, 0, sizeof(*scope));

    list_node_init(&scope->scope_entry);

    scope->variable_table.cmp = jaz_ast_variable_cmp;
    scope->variable_table.hash = jaz_ast_variable_hash;

    return scope;
}

void jaz_ast_del_scope(struct jaz_ast_scope *scope)
{
    hashtable_clear(&scope->variable_table, jaz_ast_variable_free);
    free(scope);
}

static const char *token_name(int tok)
{
    switch (tok) {
        case TOK_IDENT: return "TOK_IDENT";
        case TOK_LABEL: return "TOK_LABEL";
        case TOK_GOTO: return "TOK_GOTO";
        case TOK_GOFALSE: return "TOK_GOFALSE";
        case TOK_GOTRUE: return "TOK_GOTRUE";
        case TOK_PUSH: return "TOK_PUSH";
        case TOK_POP: return "TOK_POP";
        case TOK_RVALUE: return "TOK_RVALUE";
        case TOK_LVALUE: return "TOK_LVALUE";
        case TOK_HALT: return "TOK_HALT";
        case TOK_MOD: return "TOK_MOD";
        case TOK_NOT_EQUAL: return "TOK_NOT_EQUAL";
        case TOK_GREATER_THAN_EQUAL: return "TOK_GREATER_THAN_EQUAL";
        case TOK_LESS_THAN_EQUAL: return "TOK_LESS_THAN_EQUAL";
        case TOK_PRINT: return "TOK_PRINT";
        case TOK_SHOW: return "TOK_SHOW";
        case TOK_BEGIN: return "TOK_BEGIN";
        case TOK_END: return "TOK_END";
        case TOK_RETURN: return "TOK_RETURN";
        case TOK_CALL: return "TOK_CALL";
        case TOK_ASSIGN: return "TOK_ASSIGN";
        case TOK_COPY: return "TOK_COPY";
        case TOK_EOF: return "TOK_EOF";
        case TOK_ERR: return "TOK_ERR";
        default: printf("%d\n", tok); return "";
    }
}

void jaz_ast_clear(struct jaz_ast *ast)
{

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

int jaz_ast_resolve_labels(struct jaz_ast *ast)
{
    struct jaz_ast_entry *entry;

    list_foreach_entry(&ast->ast_list, entry, ast_entry) {
        struct jaz_ast_label_entry *label;

        if (entry->op != TOK_GOTO && entry->op != TOK_GOTRUE && entry->op != TOK_GOFALSE && entry->op != TOK_CALL)
            continue;

        entry->data.label = NULL;

        list_foreach_entry(&ast->ast_label_list, label, ast_label_entry) {
            if (strcmp(label->id, entry->data.param) == 0) {
                entry->data.label = label->location;
                break;
            }
        }

        if (!entry->data.label) {
            printf("Error: Unknown label: \"%s\"\n", entry->data.param);
            return 1;
        }
    }

    return 0;
}

void jaz_ast_print(struct jaz_ast *ast)
{
    struct jaz_ast_entry *entry;

    list_foreach_entry(&ast->ast_list, entry, ast_entry) {
        printf("Tok: %s\n", token_name(entry->op));
    }
}

void jaz_ast_run(struct jaz_ast *ast)
{
    struct jaz_ast_entry *entry = list_first_entry(&ast->ast_list, struct jaz_ast_entry, ast_entry);

    ast->halt = 0;

    ast->lvalue_scope_depth = ast->rvalue_scope_depth = 0;
    ast->lvalue_scope = ast->rvalue_scope = jaz_ast_new_scope();

    list_add_tail(&ast->scope_list, &ast->lvalue_scope->scope_entry);

    while (!ast->halt && !list_ptr_is_head(&ast->ast_list, &entry->ast_entry))
        entry = (jaz_op_lookup_table[entry->op]) (ast, entry);

    return ;
}

