#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef __WIN32
#include <malloc.h>
#endif

#include "elfload.h"
#include "elfload_dlfcn.h"
#include "elfload_exec.h"
#include "whereami.h"

int main(int argc, char **argv, char **envp)
{
    struct ELF_File *f;
    void **newstack;
    int i, envc, base;
    char *dir, *fil, *arg, *argn, *gelfshims, *shimtarget;
    char *elff = NULL;
    int elfargs;

#define ARG(s, l) if (!strcmp(arg, "-" #s) || !strcmp(arg, "--" #l))
#define ARGN(s, l) if ((!strcmp(arg, "-" #s) || !strcmp(arg, "--" #l)) && (arg = argv[++i]))

    shimtarget = "linux-gnu-libc6";
    for (i = 1; i < argc; i++) {
        arg = argv[i];
        argn = argv[i+1];
        if (arg[0] == '-') {
            ARGN(t, target) {
                shimtarget = arg;
            } else {
                fprintf(stderr, "Unrecognized argument %s.\n", arg);
            }
        } else {
            elff = arg;
            elfargs = i;
            break;
        }
    }
    if (elff == NULL) {
        fprintf(stderr, "No ELF binary specified!\n");
        return 1;
    }

    whereAmI(argv[0], &dir, &fil);
    elfload_dlinstdir = dir;

    /* load in gelfshims if applicable */
    gelfshims = malloc(strlen(dir) + strlen(shimtarget) + 1024);
    if (gelfshims == NULL) {
        perror("malloc");
        exit(1);
    }
    sprintf(gelfshims, "libhost_%s/../lib/gelfload/libgelfload-gelfshims-%s.so.0", dir, shimtarget);
    if (access(gelfshims + 8, F_OK) == 0) {
        fprintf(stderr, "Loading gelfshims from %s\n", gelfshims);
        f = loadELF(gelfshims, dir);
        f->hostlib = HOSTLIB_SHIM;
    }

    /* load them all in */
    f = loadELF(elff, dir);

    /* load native gelfshims */
    sprintf(gelfshims, "%s/../lib/gelfload/libgelfload-native-gelfshims-%s.so.0", dir, shimtarget);
    if (access(gelfshims + 8, F_OK) == 0) {
        fprintf(stderr, "Loading native gelfshims from %s\n", gelfshims);
        f = loadELF(gelfshims, dir);
    }
    free(gelfshims);

    /* relocate them */
    relocateELFs();

    /* initialize .so files */
    initELF(f);

    /* make its stack */
    for (envc = 0; envp[envc]; envc++);
    newstack = (void**)
        alloca((argc + envc + 2) * sizeof(void*));
    newstack[0] = (void*) (size_t) (argc - elfargs);
    for (i = 0; i < argc - elfargs; i++) {
        newstack[i+1] = (void*) argv[i+elfargs];
    }
    newstack[i+1] = NULL;
    base = i+2;

    for (i = 0; i < envc; i++) {
        newstack[base+i] = (void*) envp[i];
    }
    newstack[base+i] = NULL;

    /* and call it */
    WITHSTACK_JMP(newstack, f->ehdr->e_entry + f->offset);
}
