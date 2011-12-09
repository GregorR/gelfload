/* arbitrary GNU-specific functions */
#include <alloca.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <unistd.h>
#include "shim.h"

void *SHIM(mempcpy)(void *a, const void *b, size_t c)
{
    memcpy(a, b, c);
    return (void *) ((char *) a + c);
}

void *SHIM(__rawmemchr)(const void *a, int b)
{
    return memchr(a, b, (size_t) -1);
}

int SHIM(linkat)(int olddirfd, const char *oldpath,
        int newdirfd, const char *newpath, int flags)
{
    /* FIXME: flags ignored */
    int ret, curdir, path_max;
    char *actualpath;

#ifdef PATH_MAX
    path_max = PATH_MAX;
#else
    path_max = pathconf(path, _PC_PATH_MAX);
    if (path_max <= 0)
        path_max = 4096;
#endif

    actualpath = alloca(path_max + 1);

    /* not all platforms provide fstatat, so chdir */
    curdir = open(".", O_RDONLY);
    if (curdir < 0) {
        /* failed to get the current directory */
        ret = -1;
    } else {
        if (fchdir(olddirfd) >= 0 && realpath(oldpath, actualpath) &&
            fchdir(newdirfd) >= 0) {
            ret = link(actualpath, newpath);
        } else {
            ret = -1;
        }
        fchdir(curdir);
        close(curdir);
    }
    return ret;
}

