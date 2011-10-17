#include <stdlib.h>
#include "shim.h"

extern int main(int argc, char **argv, char **envp);

int SHIM(__libc_start_main)() {
    /* FIXME: this is obviously all wrong */
    char *argv[] = {"gelfload", NULL};
    char *envp[] = {NULL};
    return main(1, argv, envp);
}
