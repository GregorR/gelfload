#ifndef FILE_H
#define FILE_H

#include <stdio.h>

#include "helpers.h"
#include "shim.h"

/* shim FILE * for platforms with files in stupid places */
TSHIM(FILE) {
    FILE *hf;
};

TSHIM_NEW_D(FILE);
TSHIM_DELETE_D(FILE);
TSHIM_T2H_D(FILE);
TSHIM_H2T_D(FILE);

#endif
