
%{
#include <string.h>
#include <stdio.h>

#include "lexer.h"
#include "ast.h"
#include "parser.tab.h"

extern YYLTYPE yylloc;
#define YY_NO_INPUT

#define YY_NO_INPUT

#define YY_USER_ACTION { \
        yylloc.first_line = yylloc.last_line = yylineno; \
    }
%}

%option noyywrap
%option yylineno

%x argument

%%

\r?\n { return '\n'; }

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

"div"    { return TOK_MOD; }
"halt"   { return TOK_HALT; }
"copy"   { return TOK_COPY; }
"pop"    { return TOK_POP; }
"print"  { return TOK_PRINT; }
"begin"  { return TOK_BEGIN; }
"end"    { return TOK_END; }
"return" { return TOK_RETURN; }

 /* 
  * When interpreting arguments, we go into a separate state. Arguments can be
  * any arbitrary string until a newline, so we don't want to parse them using
  * the above rules, which would split on spaces, keywords, and some symbols.
  */
"push"    { BEGIN(argument); return TOK_PUSH; }
"rvalue"  { BEGIN(argument); return TOK_RVALUE; }
"lvalue"  { BEGIN(argument); return TOK_LVALUE; }
"goto"    { BEGIN(argument); return TOK_GOTO; }
"gotrue"  { BEGIN(argument); return TOK_GOTRUE; }
"gofalse" { BEGIN(argument); return TOK_GOFALSE; }
"label"   { BEGIN(argument); return TOK_LABEL; }
"call"    { BEGIN(argument); return TOK_CALL; }
"show"    { BEGIN(argument); return TOK_SHOW; }

<argument>[^\n\r]+ {
    /* We add one to strip off the leading space */
    yylval.str = strdup(yytext + 1);
    BEGIN(INITIAL);
    return TOK_IDENT;
}

<argument>\r?\n {
    BEGIN(INITIAL);
    return '\n';
}

[^ \t\n\r]+ {
    yylval.str = strdup(yytext);
    return TOK_IDENT;
}

<<EOF>> { return TOK_EOF; }

%%

/* Get rid of error about unused yyunput */
void fake_func(void)
{
    yyunput(0, 0);
}

