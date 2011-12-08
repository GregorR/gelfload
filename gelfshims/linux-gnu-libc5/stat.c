#include <stdio.h>
#include "shim.h"

#define STAT(x) x ## __internal
#define TSTAT_H
#define T_STAT_VER 1

TSHIM(structstat__internal) {
    unsigned short        st_dev;
    unsigned short __pad1;
    unsigned long        st_ino;
    unsigned short        st_mode;
    unsigned short        st_nlink;
    unsigned short        st_uid;
    unsigned short        st_gid;
    unsigned short        st_rdev;
    unsigned short __pad2;
    long        st_size;
    unsigned long    st_blksize;
    unsigned long    st_blocks;
    long        st_atime;
    unsigned long    __unused1;
    long        st_mtime;
    unsigned long    __unused2;
    long        st_ctime;
    unsigned long    __unused3;
    unsigned long    __unused4;
    unsigned long    __unused5;
};

#include "cshstat.c"

#define VERCHECK do { \
    if (ver != 0 && ver != T_STAT_VER) { \
        fprintf(stderr, "Unrecognized stat version %d\n", ver); \
        return -1; \
    } \
} while(0)

int SHIM(_xstat)(int ver, const char *path, TSHIM(structstat__internal) *buf)
{
    VERCHECK;
    return SHIM2(STAT(stat))(path, buf);
}

int SHIM(_fxstat)(int ver, int fd, TSHIM(structstat__internal) *buf)
{
    VERCHECK;
    return SHIM2(STAT(fstat))(fd, buf);
}

UNIMPL_SHIM(_lxstat);
