#define _POSIX_C_SOURCE 200809L /* for getdelim */
#define _GNU_SOURCE /* also for getdelim (glibc < 2.10) */

#include <alloca.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "shim.h"
#include "shstdio.h"

int SHIM(putc)(int a, TSHIM(FILE) *b);
int SHIM(getc)(TSHIM(FILE) *a);
ssize_t SHIM(getdelim)(char **a, size_t *b, int c, TSHIM(FILE) *d);

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

size_t SHIM(fwrite_unlocked)(const void *a, size_t b, size_t c, TSHIM(FILE) *d)
{
    FILE *hd = NULL;
    TSHIM_T2H(FILE)(&hd, &d);
    return fwrite(a, b, c, hd);
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

/* GNU-specific weird names for things */
int SHIM(_IO_putc)(int a, TSHIM(FILE) *b)
{
    return SHIM(putc)(a, b);
}

int SHIM(_IO_getc)(TSHIM(FILE) *a)
{
    return SHIM(getc)(a);
}

int SHIM(__getdelim)(char **a, size_t *b, int c, TSHIM(FILE) *d)
{
    return SHIM(getdelim)(a, b, c, d);
}

/* -64 */
int SHIM(fseeko64)(TSHIM(FILE) *a, int64_t b, int c)
{
    FILE *ha;
    TSHIM_T2H(FILE)(&ha, &a);
    return fseeko(ha, b, c);
}
