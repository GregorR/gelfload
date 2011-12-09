#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef __WIN32
#include <malloc.h>
#else
#include <alloca.h>
#endif

#include "../config.h"

int main(int argc, char **argv)
{
    char **newargs;

    /* form our new arguments */
    newargs = alloca((argc + 2) * sizeof(char*));
    memcpy(newargs + 1, argv, (argc + 1) * sizeof(char*));
    newargs[0] = PREFIX "/bin/gelfload";

    /* then call */
    execv(newargs[0], newargs);
    fprintf(stderr, "Failed to call %s\n", newargs[0]);
    return 0;
}
