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
    char *dir, *fil, *generishims;

    if (argc < 2) {
        fprintf(stderr, "Use: elfload <elf file> [arguments]\n");
        return 1;
    }

    whereAmI(argv[0], &dir, &fil);
    elfload_dlinstdir = dir;

    /* load in generishims if applicable */
    generishims = malloc(strlen(dir) + 64);
    if (generishims == NULL) {
        perror("malloc");
        exit(1);
    }
    sprintf(generishims, "libhost_%s/../lib/gelfload/libgelfload-generishims.so.0", dir);
    if (access(generishims + 8, F_OK) == 0) {
        loadELF(generishims, dir);
    }
    free(generishims);

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
