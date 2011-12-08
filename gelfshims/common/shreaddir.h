#ifndef SHREADDIR_H
#define SHREADDIR_H
#ifndef TREADDIR_C
#error shreaddir.h must always be included via a target readdir.c
#endif

#include <dirent.h>

#include "shim.h"

typedef struct dirent DIRENT(structdirent);

TSHIM_T2H_D(DIRENT(structdirent));
TSHIM_H2T_D(DIRENT(structdirent));

#endif
