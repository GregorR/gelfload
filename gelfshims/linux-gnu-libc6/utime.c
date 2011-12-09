/* NS-precision utime stuff */
#define _BSD_SOURCE /* for futimes */

#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <utime.h>
#include "shim.h"

/* precision is for losers */
int SHIM(futimens)(int a, const struct timespec b[2])
{
    if (b == NULL) {
        return futimes(a, NULL);
    } else {
        struct timeval tvs[2];
        tvs[0].tv_sec = b[0].tv_sec;
        tvs[0].tv_usec = b[0].tv_nsec / 1000000;
        tvs[1].tv_sec = b[1].tv_sec;
        tvs[1].tv_usec = b[1].tv_nsec / 1000000;
        return futimes(a, tvs);
    }
}
