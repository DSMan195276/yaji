
#include <stdio.h>
#include <string.h>

#include "vm.h"

int main(int argc, char **argv)
{
    struct jaz_vm vm;
    int ret;
    const char *file;

    if (argc > 1) {
        file = argv[1];
    } else {
        printf("yaji - Yet Another Jaz Interpreter\n");
        printf("Written by Matthew Kilgore and Peter Tschaekofske - Spring 2017\n");
        printf("\n");
        printf("Usage: %s jaz_file\n", argv[0]);
        return 0;
    }

    jaz_vm_init(&vm);

    ret = jaz_vm_load(&vm, file);
    if (ret)
        return 0;

    jaz_vm_run(&vm);

    jaz_vm_clear(&vm);

    return 0;
}

