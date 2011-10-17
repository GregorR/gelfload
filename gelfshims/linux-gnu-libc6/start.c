#include <stdio.h>
#include <stdlib.h>
#include "shim.h"

int SHIM(__libc_start_main)(int (*main) (int, char **, char **), int argc, char **argv) {
    char **envp;
    int i;
    for (i = 0; argv[i]; i++);
    envp = argv + i + 1;
    return main(argc, argv, envp);
}
