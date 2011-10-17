#include <alloca.h>
#include <stdio.h>
#include <string.h>
#include "shim.h"
#include "shstdio.h"

/* stdin/stdout/stderr are just globals */
TSHIM(FILE) *SHIM(stdin), *SHIM(stdout), *SHIM(stderr);

CONSTRUCTOR(void shim_init_stdio())
{
    SHIM(stdin) = TSHIM_NEW(FILE)();
    SHIM(stdin)->hf = stdin;

    SHIM(stdout) = TSHIM_NEW(FILE)();
    SHIM(stdout)->hf = stdout;

    SHIM(stderr) = TSHIM_NEW(FILE)();
    SHIM(stderr)->hf = stderr;
}

/* GNU-specific unlocked things */
int SHIM(fputs_unlocked)(const char *a, TSHIM(FILE) *b)
{
    FILE *hb = NULL;
    TSHIM_T2H(FILE)(&hb, &b);
    return fputs(a, hb);
}
