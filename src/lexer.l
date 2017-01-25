
%{
#include <string.h>
#include <stdio.h>

#include "lexer.h"
#include "ast.h"
#include "parser.tab.h"

#define YY_NO_INPUT
#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;
%}

%option noyywrap
%option yylineno

%%

\n { return '\n'; }

[ \t]+ /* White space */

"+" { return '+'; }
"-" { return '-'; }
"*" { return '*'; }
"/" { return '/'; }
"&" { return '&'; }
"|" { return '|'; }
"!" { return '!'; }

":=" { return TOK_ASSIGN; }
"<>" { return TOK_NOT_EQUAL; }
">=" { return TOK_GREATER_THAN_EQUAL; }
"<=" { return TOK_LESS_THAN_EQUAL; }
"="  { return '='; }
"<"  { return '<'; }
">"  { return '>'; }

"div"     { return TOK_MOD; }
"halt"    { return TOK_HALT; }
"gotrue"  { return TOK_GOTRUE; }
"gofalse" { return TOK_GOFALSE; }
"rvalue"  { return TOK_RVALUE; }
"lvalue"  { return TOK_LVALUE; }
"copy"    { return TOK_COPY; }
"pop"     { return TOK_POP; }
"push"    { return TOK_PUSH; }
"goto"    { return TOK_GOTO; }
"show"    { return TOK_SHOW; }
"print"   { return TOK_PRINT; }
"label"   { return TOK_LABEL; }

"begin"  { return TOK_BEGIN; }
"end"    { return TOK_END; }
"return" { return TOK_RETURN; }
"call"   { return TOK_CALL; }

[a-zA-Z0-9_\-]+ {
    yylval.str = strdup(yytext);
    return TOK_IDENT;
}

<<EOF>> { return TOK_EOF; }

. { return TOK_ERR; }

%%

/* Get rid of error about unused yyunput */
void fake_func(void)
{
    yyunput(0, 0);
}

