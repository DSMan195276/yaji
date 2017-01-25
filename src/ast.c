
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "ast.h"

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

    fclose(filp);
    return ret;
}

int jaz_ast_resolve_labels(struct jaz_ast *ast)
{
    struct jaz_ast_entry *entry;

    list_foreach_entry(&ast->ast_list, entry, ast_entry) {
        struct jaz_ast_label_entry *label;

        if (entry->op != TOK_GOTO && entry->op != TOK_GOTRUE && entry->op != TOK_GOFALSE)
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

#define jaz_ast_next_entry(ent) \
    list_next_entry(ent, ast_entry)

static struct jaz_ast_entry *jaz_ast_run_entry(struct jaz_ast *ast, struct jaz_ast_entry *entry)
{
    switch (entry->op) {
    case TOK_GOTO:
        return entry->data.label;

    case TOK_PUSH:
        ast->stack_top++;
        *ast->stack_top = atoi(entry->data.param);
        break;

    case TOK_POP:
        ast->stack_top--;
        break;

    case '+':
        ast->stack_top[-1] = ast->stack_top[0] + ast->stack_top[-1];
        ast->stack_top--;
        break;

    case '-':
        ast->stack_top[-1] = ast->stack_top[0] - ast->stack_top[-1];
        ast->stack_top--;
        break;

    case '*':
        ast->stack_top[-1] = ast->stack_top[0] * ast->stack_top[-1];
        ast->stack_top--;
        break;

    case '/':
        ast->stack_top[-1] = ast->stack_top[0] / ast->stack_top[-1];
        ast->stack_top--;
        break;

    case TOK_MOD:
        ast->stack_top[-1] = ast->stack_top[0] % ast->stack_top[-1];
        ast->stack_top--;
        break;

    case TOK_SHOW:
        printf("%s\n", entry->data.param);
        break;

    case TOK_PRINT:
        printf("%d\n", *ast->stack_top);
        break;

    default:
        break;
    }

    return list_next_entry(entry, ast_entry);
}

void jaz_ast_run(struct jaz_ast *ast)
{
    struct jaz_ast_entry *entry = list_first_entry(&ast->ast_list, struct jaz_ast_entry, ast_entry);

    while (!list_ptr_is_head(&ast->ast_list, &(entry = jaz_ast_run_entry(ast, entry))->ast_entry))
        ;

    return ;
}

