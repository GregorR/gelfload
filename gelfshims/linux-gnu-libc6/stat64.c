#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "shim.h"

#define STAT(x) x ## 64__internal
#define TSTAT_H

/* FIXME */
#ifndef __WORDSIZE
#define __WORDSIZE 32
#endif

/* expected versions */
#if __WORDSIZE == 32
#define T_STAT_VER 3
#else
#define T_STAT_VER 1
#endif

/* stat64 version 1 on GNU/Linux */
TSHIM(structstat64__internal) {
    unsigned long long st_dev;
#if __WORDSIZE == 64
    unsigned long st_ino;
    unsigned long st_nlink;
    unsigned int st_mode;
#else
    unsigned int __pad1;
    unsigned long __st_ino;
    unsigned int st_mode;
    unsigned long st_nlink;
#endif
    unsigned int st_uid;
    unsigned int st_gid;
#if __WORDSIZE == 64
    int __pad0;
    unsigned int st_rdev;
    long st_size;
#else
    unsigned long long st_rdev;
    unsigned int __pad2;
    long long st_size;
#endif
    long st_blksize;
    long long st_blocks;
    long __st_atime;
    unsigned long st_atimensec;
    long __st_mtime;
    unsigned long st_mtimensec;
    long __st_ctime;
    unsigned long st_ctimensec;
#if __WORDSIZE == 64
    long int __unused[3];
#else
    unsigned long long st_ino;
#endif
};

#include "cshstat.c"

#define VERCHECK do { \
    if (ver != 0 && ver != T_STAT_VER) { \
        fprintf(stderr, "Unrecognized stat version %d\n", ver); \
        return -1; \
    } \
} while(0)

int SHIM(__xstat64)(int ver, const char *path, TSHIM(structstat64__internal) *buf)
{
    VERCHECK;
    return SHIM2(STAT(stat))(path, buf);
}

int SHIM(__fxstat64)(int ver, int fd, TSHIM(structstat64__internal) *buf)
{
    VERCHECK;
    return SHIM2(STAT(fstat))(fd, buf);
}

int SHIM(__lxstat64)(int ver, const char *path, TSHIM(structstat64__internal) *buf)
{
    VERCHECK;
    return SHIM2(STAT(lstat))(path, buf);
}
