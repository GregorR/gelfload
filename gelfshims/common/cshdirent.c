#define _SVID_SOURCE /* for dirfd */

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shim.h"
#include "shdirent.h"

TSHIM_NEW_D(DIR) {
    TSHIM(DIR) *ret;
    SF(ret, calloc, NULL, (sizeof(TSHIM(DIR)) + 256, 1)); /* padded for weird OSes */
    ret->hf = NULL;
}

TSHIM_DELETE_D(DIR) {
    free(t);
}

TSHIM_T2H_D(DIR) {
    *h = (*t)->hf;
}

TSHIM_T2HP_D(DIR) {}

TSHIM_H2T_D(DIR) {
    *t = TSHIM_NEW(DIR)();
    (*t)->hf = *h;
}

int SHIM(closedir)(TSHIM(DIR) *a)
{
    DIR *ha;
    TSHIM_T2H(DIR)(&ha, &a);
    return closedir(ha);
}

TSHIM(DIR) *SHIM(opendir)(const char *a)
{
    DIR *hr;
    TSHIM(DIR) *tr = NULL;
    hr = opendir(a);
    if (hr) {
        tr = TSHIM_NEW(DIR)();
        TSHIM_H2T(FILE)(&tr, &hr);
    }
    return tr;
}

void SHIM(rewinddir)(TSHIM(DIR) *a)
{
    DIR *ha;
    TSHIM_T2H(DIR)(&ha, &a);
    rewinddir(ha);
}

void SHIM(seekdir)(TSHIM(DIR) *a, long b)
{
    DIR *ha;
    TSHIM_T2H(DIR)(&ha, &a);
    seekdir(ha, b);
}

long SHIM(telldir)(TSHIM(DIR) *a)
{
    DIR *ha;
    TSHIM_T2H(DIR)(&ha, &a);
    return telldir(ha);
}

int SHIM(dirfd)(TSHIM(DIR) *a)
{
    DIR *ha;
    TSHIM_T2H(DIR)(&ha, &a);
    return dirfd(ha);
}
