#ifndef SHDIRENT_H
#define SHDIRENT_H

#include <stdio.h>

#include "helpers.h"
#include "shim.h"

/* shim DIR * for platforms with files in stupid places */
TSHIM(DIR) {
    void *junk[3]; /* to avoid overwrites on weird OSes */
    DIR *hf;
};

TSHIM_NEW_D(DIR);
TSHIM_DELETE_D(DIR);
TSHIM_T2H_D(DIR);
TSHIM_T2HP_D(DIR);
TSHIM_H2T_D(DIR);

#endif
