#include <fcntl.h>
#include "shim.h"

#define TOFLAG(x) if (oflag & TO_ ## x) hoflag |= O_ ## x;

#ifndef O_RSYNC
#define O_RSYNC O_SYNC
#endif

/* shared open that translates oflags */
int SHIM(open)(const char *path, int oflag, int mode)
{
    int hoflag = 0;
    TOFLAG(RDONLY);
    TOFLAG(WRONLY);
    TOFLAG(RDWR);
    TOFLAG(APPEND);
    TOFLAG(CREAT);
    TOFLAG(DSYNC);
    TOFLAG(EXCL);
    TOFLAG(NOCTTY);
    TOFLAG(NONBLOCK);
    TOFLAG(RSYNC);
    TOFLAG(SYNC);
    TOFLAG(TRUNC);
    return open(path, hoflag, mode);
}
