#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include "shim.h"

#define DIRENT(x) x ## 64
#define TREADDIR_C

TSHIM(structdirent64) {
    unsigned long long d_ino;
    long long d_off;
    unsigned short int d_reclen;
    unsigned char d_type;
    char d_name[256];
};

#include "cshreaddir.c"
