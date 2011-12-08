#include <sys/timeb.h>
#include <time.h>
#include "shim.h"

#ifndef CLOCK_REALTIME

/* this system does not seem to support realtime, give a poor implementation */

int SHIM(clock_getres)(int clk_id, struct timespec *res)
{
    res->tv_sec = 0;
    res->tv_nsec = 1000000;
    return 0;
}

int SHIM(clock_gettime)(int clk_id, struct timespec *tp)
{
    /* you totally asked for ftime, right? No? Too bad. */
    struct timeb tbp;
    if (ftime(&tbp) == 0) {
        tp->tv_sec = tbp.time;
        tp->tv_nsec = tbp.millitm * 1000000;
        return 0;
    }
    return -1;
}

#endif
