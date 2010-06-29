/* dlfcn for elfload-loaded ELFs */

#ifndef ELFLOAD_DLFCN_H
#define ELFLOAD_DLFCN_H

#include "elfload_microcosm.h"

MICROCOSM_CALLING_CONVENTION(void *elfload_dlopen(const char *filename, int flag));
MICROCOSM_CALLING_CONVENTION(char *elfload_dlerror(void));
MICROCOSM_CALLING_CONVENTION(void *elfload_dlsym(void *handle, const char *symbol));
MICROCOSM_CALLING_CONVENTION(int elfload_dlclose(void *handle));

/* return one of these functions by their name */
void *elfload_dl(const char *fname);

#endif
