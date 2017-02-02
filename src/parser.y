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
%type <ast_entry> label

%start jaz_file

%%

 /*
  * Labels are a special operation because besides creating an ast entry, they
  * also create an entry in the label list.
  *
  * The label list is used later in a pass of the AST to resolve labels into
  * pointesr to other AST entries.
  */
label:
    TOK_LABEL TOK_IDENT {
        struct jaz_label *label = malloc(sizeof(*label));

        $$ = jaz_ast_entry_new(TOK_LABEL, $2);

        memset(label, 0, sizeof(*label));
        label->id = strdup($2);
        label->location = $$;

        jaz_label_table_add(&ast->label_table, label);
    }
    ;

operation:
    label
    | TOK_SHOW TOK_IDENT     { $$ = jaz_ast_entry_new(TOK_SHOW,               $2); }
    | TOK_SHOW               { $$ = jaz_ast_entry_new(TOK_SHOW,               strdup("\n")); }
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

