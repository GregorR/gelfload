#include <dirent.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "shim.h"

#define DIRENT(x) x ## 64
#define TREADDIR_C

TSHIM(structdirent64) {
    uint64_t d_ino;
    int64_t d_off;
    unsigned short int d_reclen;
    unsigned char d_type;
    char d_name[256];
};

#include "cshreaddir.c"
