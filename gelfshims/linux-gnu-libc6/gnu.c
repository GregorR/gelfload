/* arbitrary GNU-specific functions */
#include <string.h>
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
