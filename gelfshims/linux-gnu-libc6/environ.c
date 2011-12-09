#include "shim.h"

char **SHIM(__environ);

#ifdef __APPLE__
#include <crt_externs.h>
#else
extern char **environ;
#endif

CONSTRUCTOR(static void init_environ())
{
#ifdef __APPLE__
    SHIM(__environ) = *_NSGetEnviron();
#else
    SHIM(__environ) = environ;
#endif
}
