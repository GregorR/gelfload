#include <stdlib.h>

#include "elfload.h"
#include "elfload_dlfcn.h"

char *elfload_dlinstdir = NULL;
char *dlLastError = NULL;

void *elfload_dlopen(const char *filename, int flag)
{
    /* pretty simple, just load based on the file name */
    struct ELF_File *f = loadELF(filename, elfload_dlinstdir, 0);
    
    if (f == NULL) {
        dlLastError = "Could not find or load file.";
    }

    return (void *) f;
}

char *elfload_dlerror(void) { return dlLastError; }

void *elfload_dlsym(void *handle, const char *symbol)
{
    void *sym = findELFSymbol(symbol, (struct ELF_File *) handle, -1, -1, NULL);
    
    if (sym == NULL) {
        dlLastError = "Symbol undefined.";
    }

    return sym;
}

int elfload_dlclose(void *handle) {return 0;}


void *elfload_dl(const char *fname)
{
    if (strcmp(fname, "dlopen") == 0) {
        return (void *) elfload_dlopen;

    } else if (strcmp(fname, "dlerror") == 0) {
        return (void *) elfload_dlerror;

    } else if (strcmp(fname, "dlsym") == 0) {
        return (void *) elfload_dlsym;

    } else if (strcmp(fname, "dlclose") == 0) {
        return (void *) elfload_dlclose;

    }

    return NULL;
}
