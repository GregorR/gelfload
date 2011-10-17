#include <alloca.h>
#include <stdio.h>
#include <string.h>
#include "shim.h"
#include "shstdio.h"

/* Solaris puts std* in __iob, and has the files directly there */
struct notQuiteAFile {
    TSHIM(FILE) tf;
};
TSHIM(FILE) SHIM(__iob)[4];

CONSTRUCTOR(void shim_init__iob())
{
    memset(SHIM(__iob), 0, sizeof(SHIM(__iob)));
    SHIM(__iob)[0].hf = stdin;
    SHIM(__iob)[1].hf = stdout;
    SHIM(__iob)[2].hf = stderr;
}

int SHIM(__flsbuf)(int a, TSHIM(FILE) *b)
{
    FILE *hb = NULL;
    TSHIM_T2H(FILE)(&hb, &b);
    return fflush(hb);
}

/* fputs on Solaris adds a \n */
int SHIM(fputs)(const char *a, TSHIM(FILE) *b)
{
    FILE *hb = NULL;
    char *ha = alloca(strlen(a) + 2);
    TSHIM_T2H(FILE)(&hb, &b);
    sprintf(ha, "%s\n", a);
    return fputs(ha, hb);
}
