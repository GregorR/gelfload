#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shim.h"

/* this is a weird, GNU-specific function * /
void SHIM(error)(int status, int errnum, const char *format, ...)
{
    va_list vl;
    va_start(vl, format);

    fflush(stdout);
    vfprintf(stderr, format, vl);
    if (errnum != 0) {
        fprintf(stderr, ": %s", strerror(errnum));
    }
    fprintf(stderr, "\n");
}
*/
