#include <string.h>
#include "shstat.h"

#define SHIM2(x) SHIM(x)
#define TSHIM2(x) TSHIM(x)
#define TSHIM_H2T2(x) TSHIM_H2T(x)

TSHIM_T2H_D(STAT(structstat))
{
    memset(*h, 0, sizeof(**h));
    (*h)->st_dev = (*t)->st_dev;
    (*h)->st_ino = (*t)->st_ino;
    (*h)->st_mode = (*t)->st_mode;
    (*h)->st_nlink = (*t)->st_nlink;
    (*h)->st_uid = (*t)->st_uid;
    (*h)->st_gid = (*t)->st_gid;
    (*h)->st_rdev = (*t)->st_rdev;
    (*h)->st_size = (*t)->st_size;
}

TSHIM_H2T_D(STAT(structstat))
{
    memset(*t, 0, sizeof(**t));
    (*t)->st_dev = (*h)->st_dev;
    (*t)->st_ino = (*h)->st_ino;
    (*t)->st_mode = (*h)->st_mode;
    (*t)->st_nlink = (*h)->st_nlink;
    (*t)->st_uid = (*h)->st_uid;
    (*t)->st_gid = (*h)->st_gid;
    (*t)->st_rdev = (*h)->st_rdev;
    (*t)->st_size = (*h)->st_size;
}

int SHIM2(STAT(stat))(const char *a, TSHIM2(STAT(structstat)) *b)
{
    STAT(structstat) hb;
    STAT(structstat) *hbp = &hb;
    int ret = stat(a, hbp);
    TSHIM_H2T2(STAT(structstat))(&b, &hbp);
    return ret;
}

int SHIM2(STAT(fstat))(int a, TSHIM2(STAT(structstat)) *b)
{
    STAT(structstat) hb;
    STAT(structstat) *hbp = &hb;
    int ret = fstat(a, hbp);
    TSHIM_H2T2(STAT(structstat))(&b, &hbp);
    return ret;
}

int SHIM2(STAT(lstat))(const char *a, TSHIM2(STAT(structstat)) *b)
{
    STAT(structstat) hb;
    STAT(structstat) *hbp = &hb;
    int ret = stat(a, hbp);
    TSHIM_H2T2(STAT(structstat))(&b, &hbp);
    return ret;
}

int SHIM2(STAT(fstatat))(int a, const char *b, TSHIM2(STAT(structstat)) *c, int d)
{
    STAT(structstat) hc;
    STAT(structstat) *hcp = &hc;
    int ret = fstatat(a, b, hcp, d);
    TSHIM_H2T2(STAT(structstat))(&c, &hcp);
    return ret;
}

