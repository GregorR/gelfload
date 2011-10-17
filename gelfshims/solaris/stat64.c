#include "shim.h"

#define STAT(x) x ## 64
#define TSTAT_H

struct soltimespec {
    long a, b;
};

TSHIM(structstat64) {
    unsigned long st_dev;
    unsigned long st_ino;
    unsigned long st_mode;
    unsigned long st_nlink;
    long st_uid;
    long st_gid;
    unsigned long st_rdev;
    long st_size;
    struct soltimespec times[3];
    long st_blksize;
    long st_blocks;
    char st_fstype[16];
};

#include "cshstat.c"
