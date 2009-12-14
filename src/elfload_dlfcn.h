/* dlfcn for elfload-loaded ELFs */

#ifndef ELFLOAD_DLFCN_H
#define ELFLOAD_DLFCN_H

void *elfload_dlopen(const char *filename, int flag);
char *elfload_dlerror(void);
void *elfload_dlsym(void *handle, const char *symbol);
int elfload_dlclose(void *handle);

/* return one of these functions by their name */
void *elfload_dl(const char *fname);

#endif
