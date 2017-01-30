%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lexer.h"

#define IN_PARSER
#include "ast.h"

void yyerror(struct jaz_ast *, const char *str);

static struct jaz_ast_entry *jaz_ast_entry_new(int op, char *parameter);

#define YYERROR_VERBOSE
%}

%union {
    char *str;
    struct jaz_ast_entry *ast_entry;
}

%parse-param { struct jaz_ast *ast }
%locations

%token <str> TOK_IDENT
%type  <str> show_ident_list

%token TOK_LABEL TOK_GOTO TOK_GOFALSE TOK_GOTRUE
%token TOK_PUSH TOK_POP
%token TOK_RVALUE TOK_LVALUE
%token TOK_ASSIGN TOK_COPY
%token TOK_HALT
%token TOK_MOD
%token TOK_NOT_EQUAL TOK_GREATER_THAN_EQUAL TOK_LESS_THAN_EQUAL
%token TOK_PRINT TOK_SHOW
%token TOK_BEGIN TOK_END TOK_RETURN TOK_CALL
%token TOK_EOF TOK_ERR

%type <ast_entry> operation statement
%type <ast_entry> label show

%start jaz_file

%%

label:
    TOK_LABEL TOK_IDENT {
        struct jaz_ast_label_entry *label = malloc(sizeof(*label));

        $$ = jaz_ast_entry_new(TOK_LABEL, $2);

        jaz_ast_label_entry_init(label);
        label->id = strdup($2);
        label->location = $$;

        list_add_tail(&ast->ast_label_list, &label->ast_label_entry);
     }
     ;

show_ident_list: TOK_IDENT {
        $$ = strdup($1);
    }
    | show_ident_list TOK_IDENT {
        size_t len1 = strlen($1);
        size_t len2 = strlen($2);
        size_t total_len = len1 + 1 + len2 + 1;
        
        if (len1 == 0) {
            free($1);
            $$ = $2;
            break;
        }

        char *result = malloc(total_len);
        memset(result, 0, total_len);

        memcpy(result, $1, len1);

        result[len1] = ' ';
        len1++;

        memcpy(result + len1, $2, len2);

        free($1);
        free($2);

        $$ = result;
    }
    ;

show:
    TOK_SHOW show_ident_list { $$ = jaz_ast_entry_new(TOK_SHOW, $2); }
    | TOK_SHOW               { $$ = jaz_ast_entry_new(TOK_SHOW, strdup("\n")); }
    ;

operation:
    label
    | show
    | TOK_GOTO TOK_IDENT     { $$ = jaz_ast_entry_new(TOK_GOTO,               $2); }
    | TOK_PUSH TOK_IDENT     { $$ = jaz_ast_entry_new(TOK_PUSH,               $2); }
    | TOK_POP                { $$ = jaz_ast_entry_new(TOK_POP,                NULL); }
    | TOK_RVALUE TOK_IDENT   { $$ = jaz_ast_entry_new(TOK_RVALUE,             $2); }
    | TOK_LVALUE TOK_IDENT   { $$ = jaz_ast_entry_new(TOK_LVALUE,             $2); }
    | TOK_GOFALSE TOK_IDENT  { $$ = jaz_ast_entry_new(TOK_GOFALSE,            $2); }
    | TOK_GOTRUE TOK_IDENT   { $$ = jaz_ast_entry_new(TOK_GOTRUE,             $2); }
    | TOK_ASSIGN             { $$ = jaz_ast_entry_new(TOK_ASSIGN,             NULL); }
    | TOK_COPY               { $$ = jaz_ast_entry_new(TOK_COPY,               NULL); }
    | TOK_HALT               { $$ = jaz_ast_entry_new(TOK_HALT,               NULL); }
    | '+'                    { $$ = jaz_ast_entry_new('+',                    NULL); }
    | '-'                    { $$ = jaz_ast_entry_new('-',                    NULL); }
    | '*'                    { $$ = jaz_ast_entry_new('*',                    NULL); }
    | '/'                    { $$ = jaz_ast_entry_new('/',                    NULL); }
    | TOK_MOD                { $$ = jaz_ast_entry_new(TOK_MOD,                NULL); }
    | '&'                    { $$ = jaz_ast_entry_new('&',                    NULL); }
    | '!'                    { $$ = jaz_ast_entry_new('!',                    NULL); }
    | '|'                    { $$ = jaz_ast_entry_new('|',                    NULL); }
    | TOK_NOT_EQUAL          { $$ = jaz_ast_entry_new(TOK_NOT_EQUAL,          NULL); }
    | '='                    { $$ = jaz_ast_entry_new('=',                    NULL); }
    | '<'                    { $$ = jaz_ast_entry_new('<',                    NULL); }
    | TOK_LESS_THAN_EQUAL    { $$ = jaz_ast_entry_new(TOK_LESS_THAN_EQUAL,    NULL); }
    | '>'                    { $$ = jaz_ast_entry_new('>',                    NULL); }
    | TOK_GREATER_THAN_EQUAL { $$ = jaz_ast_entry_new(TOK_GREATER_THAN_EQUAL, NULL); }
    | TOK_PRINT              { $$ = jaz_ast_entry_new(TOK_PRINT,              NULL); }
    | TOK_BEGIN              { $$ = jaz_ast_entry_new(TOK_BEGIN,              NULL); }
    | TOK_END                { $$ = jaz_ast_entry_new(TOK_END,                NULL); }
    | TOK_RETURN             { $$ = jaz_ast_entry_new(TOK_RETURN,             NULL); }
    | TOK_CALL TOK_IDENT     { $$ = jaz_ast_entry_new(TOK_CALL,               $2); }
    ;

statement:
    operation '\n'
    | operation TOK_EOF
    ;

jaz_file: %empty 
    | jaz_file '\n' /* Empty line */
    | jaz_file statement {
        list_add_tail(&ast->ast_list, &$2->ast_entry);
    }
    | jaz_file TOK_EOF {
        YYACCEPT;
    }
    | jaz_file TOK_ERR {
        YYABORT;
    }
    ;

%%

void yyerror(struct jaz_ast *ast, const char *str)
{
    fprintf(stderr, "Parser error: %d: %s\n", yylloc.first_line, str);
}

static struct jaz_ast_entry *jaz_ast_entry_new(int op, char *parameter)
{
    struct jaz_ast_entry *entry = malloc(sizeof(*entry));
    jaz_ast_entry_init(entry);

    entry->op = op;
    entry->data.param = parameter;

    return entry;
}

