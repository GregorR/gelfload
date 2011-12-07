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

/* __overflow is a funny name for fputc_unlocked */
int SHIM(__overflow)(TSHIM(FILE) *a, int b)
{
    FILE *ha = NULL;
    TSHIM_T2H(FILE)(&ha, &a);
    return fputc(b, ha);
}

/* __uflow is a funny name for getc_unlocked */
int SHIM(__uflow)(TSHIM(FILE) *a)
{
    FILE *ha = NULL;
    TSHIM_T2H(FILE)(&ha, &a);
    return getc(ha);
}
