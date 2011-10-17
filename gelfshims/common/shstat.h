#ifndef SHSTAT_H
#define SHSTAT_H
#ifndef TSTAT_H
#error shstat.h must always be included via a target stat.h
#endif

#include <sys/stat.h>

#include "shim.h"

typedef struct stat STAT(structstat);

TSHIM_T2H_D(STAT(structstat));
TSHIM_H2T_D(STAT(structstat));

#endif
