#include <stdlib.h>

#include "shstdio.h"

TSHIM_NEW_D(FILE) {
    TSHIM(FILE) *ret;
    SF(ret, malloc, NULL, (sizeof(TSHIM(FILE))));
    ret->hf = NULL;
}

TSHIM_DELETE_D(FILE) {
    free(t);
}

TSHIM_T2H_D(FILE) {
    *h = (*t)->hf;
}

TSHIM_H2T_D(FILE) {
    *t = TSHIM_NEW(FILE)();
    (*t)->hf = *h;
}
