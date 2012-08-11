#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../config.h"
#include "whereami.h"

int main(int argc, char **argv)
{
    char **newargs;

    /* form our new arguments */
    newargs = alloca((argc + 2) * sizeof(char*));
    memcpy(newargs + 1, argv, (argc + 1) * sizeof(char*));
    newargs[0] = PREFIX "/bin/gelfload";

    /* make the program name absolute */
    if (argv[0][0] != '/') {
        char *dir, *fil;
        if (whereAmI(argv[0], &dir, &fil)) {
            char *newargv1 = alloca(strlen(dir) + strlen(fil) + 2);
            sprintf(newargv1, "%s/%s", dir, fil);
            newargs[1] = newargv1;
        }
    }

    /* then call */
    execv(newargs[0], newargs);
    fprintf(stderr, "Failed to call %s\n", newargs[0]);
    return 0;
}
