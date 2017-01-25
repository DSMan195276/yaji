
#include <stdio.h>
#include <string.h>

#include "ast.h"

int main(int argc, char **argv)
{
    struct jaz_ast ast;
    int ret;

    jaz_ast_init(&ast);

    ret = jaz_ast_create(&ast, "./test2.jaz");

    if (ret)
        return 0;

    ret = jaz_ast_resolve_labels(&ast);
    if (ret)
        return 0;

    jaz_ast_run(&ast);

    jaz_ast_clear(&ast);

    return 0;
}
