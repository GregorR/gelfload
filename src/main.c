#include <stdio.h>
#include <stdlib.h>
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
    int i, envc;
    char *dir, *fil, *gelfshims;

    if (argc < 2) {
        fprintf(stderr, "Use: elfload <elf file> [arguments]\n");
        return 1;
    }

    whereAmI(argv[0], &dir, &fil);
    elfload_dlinstdir = dir;

    /* load in gelfshims if applicable */
    gelfshims = malloc(strlen(dir) + 1024);
    if (gelfshims == NULL) {
        perror("malloc");
        exit(1);
    }
    sprintf(gelfshims, "libhost_%s/../lib/gelfload/libgelfload-gelfshims-linux-gnu-libc6.so.0", dir);
    if (access(gelfshims + 8, F_OK) == 0) {
        fprintf(stderr, "Loading gelfshims from %s\n", gelfshims);
        loadELF(gelfshims, dir);
    }
    free(gelfshims);

    /* load them all in */
    f = loadELF(argv[1], dir);

    /* relocate them */
    relocateELFs();

    /* initialize .so files */
    initELF(f);

    /* make its stack */
    for (envc = 0; envp[envc]; envc++);
    newstack = (void**)
        alloca((argc + envc + 2) * sizeof(void*));
    newstack[0] = (void*) (size_t) (argc - 1);
    for (i = 1; i < argc; i++) {
        newstack[i] = (void*) argv[i];
    }
    newstack[i] = NULL;

    for (i = 0; i < envc; i++) {
        newstack[i+argc+1] = (void*) envp[i];
    }
    newstack[i+argc+1] = NULL;

    /* and call it */
    WITHSTACK_JMP(newstack, f->ehdr->e_entry + f->offset);
}
