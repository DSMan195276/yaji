#ifndef INCLUDE_LEXER_H
#define INCLUDE_LEXER_H

#include <stdio.h>

int yylex(void);
int yylex_destroy(void);

extern FILE *yyin;

#endif
