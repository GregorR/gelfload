#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "shdirent.h"
#include "shreaddir.h"

#define SHIM2(x) SHIM(x)
#define TSHIM2(x) TSHIM(x)
#define TSHIM_H2T2(x) TSHIM_H2T(x)

#ifndef MIN
#define MIN(x, y) (((x) < (y)) ? (x) ? (y))
#endif

TSHIM_T2H_D(DIRENT(structstat))
{
    memset(*h, 0, sizeof(**h));
    (*h)->d_ino = (*t)->d_ino;
    strncpy((*h)->d_name, (*t)->d_name, MIN(sizeof((*h)->d_name), sizeof((*t)->d_name)));
}

TSHIM_H2T_D(DIRENT(structstat))
{
    memset(*t, 0, sizeof(**t));
    (*t)->d_ino = (*h)->d_ino;
    strncpy((*t)->d_name, (*h)->d_name, MIN(sizeof((*t)->d_name), sizeof((*h)->d_name)));
}

TSHIM2(DIRENT(structdirent)) *SHIM2(DIRENT(readdir))(TSHIM(DIR) *a)
{
    DIR *ha;
    static TSHIM2(DIRENT(structdirent)) trb;
    TSHIM2(DIRENT(structdirent)) *tr = NULL;
    DIRENT(structdirent) *hr;

    TSHIM_T2H(&ha, &a);
    hr = readdir(ha);
    if (hr) {
        tr = &trb;
        TSHIM_H2T(DIRENT(structdirent))(&tr, &hr);
    }

    return tr;
}
