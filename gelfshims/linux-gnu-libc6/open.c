#define _XOPEN_SOURCE 500 /* for O_DSYNC */

#define TO_RDONLY           00
#define TO_WRONLY           01
#define TO_RDWR             02
#define TO_CREAT            0100
#define TO_EXCL             0200
#define TO_NOCTTY           0400
#define TO_TRUNC            01000
#define TO_APPEND           02000
#define TO_NONBLOCK         04000
#define TO_NDELAY           TO_NONBLOCK
#define TO_SYNC             010000
#define TO_DSYNC            TO_SYNC
#define TO_RSYNC            TO_SYNC
#define TO_FSYNC            TO_SYNC
#define TO_ASYNC            020000

#include "cshopen.c"

int SHIM(open64)(const char *path, int oflags, int mode) { return SHIM(open)(path, oflags, mode); }
