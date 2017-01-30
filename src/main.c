
#include <stdio.h>
#include <string.h>

#include "ast.h"

static struct jaz_ast ast;

int main(int argc, char **argv)
{
    int ret;
    const char *file = "./test4.jaz";

    if (argc > 1)
        file = argv[1];

    jaz_ast_init(&ast);

    ret = jaz_ast_create(&ast, file);

    if (ret)
        return 0;

    ret = jaz_ast_resolve_labels(&ast);
    if (ret)
        return 0;

    jaz_ast_run(&ast);

    jaz_ast_clear(&ast);

    return 0;
}
