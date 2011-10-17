#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shim.h"
#include "shstdio.h"

#include "redirect.h"

TSHIM_NEW_D(FILE) {
    TSHIM(FILE) *ret;
    SF(ret, malloc, NULL, (sizeof(TSHIM(FILE))));
    ret->hf = NULL;
}

TSHIM_DELETE_D(FILE) {
    free(t);
}

TSHIM_T2H_D(FILE) {
    *h = (*t)->hf;
}

TSHIM_T2HP_D(FILE) {}

TSHIM_H2T_D(FILE) {
    *t = TSHIM_NEW(FILE)();
    (*t)->hf = *h;
}

int SHIM(fflush)(TSHIM(FILE) *a)
{
    FILE *ha = NULL;
    TSHIM_T2H(FILE)(&ha, &a);
    return fflush(ha);
}

size_t SHIM(__fpending)(TSHIM(FILE) *a)
{
#ifdef __GLIBC__
    FILE *ha = NULL;
    TSHIM_T2H(FILE)(&ha, &a);
    return __fpending(ha);
#else
    return 0;
#endif
}

int SHIM(fclose)(TSHIM(FILE) *a)
{
    FILE *ha = NULL;
    TSHIM_T2H(FILE)(&ha, &a);
    return fclose(ha);
}

int SHIM(fprintf)(TSHIM(FILE) *a, const char *b, ...)
{
    FILE *ha = NULL;
    va_list c;
    TSHIM_T2H(FILE)(&ha, &a);
    va_start(c, b);
    return vfprintf(ha, b, c);
}

int SHIM(vfprintf)(TSHIM(FILE) *a, const char *b, va_list c)
{
    FILE *ha = NULL;
    TSHIM_T2H(FILE)(&ha, &a);
    return vfprintf(ha, b, c);
}

#ifndef REDIRECT_FPUTS
int SHIM(fputs)(const char *a, TSHIM(FILE) *b)
{
    FILE *hb = NULL;
    char *ha = alloca(strlen(a) + 2);
    TSHIM_T2H(FILE)(&hb, &b);
    sprintf(ha, "%s\n", a);
    return fputs(ha, hb);
}
#endif

TSHIM(FILE) *SHIM(fopen)(const char *a, const char *b)
{
    FILE *hr;
    TSHIM(FILE) *tr = NULL;
    hr = fopen(a, b);
    if (hr) {
        tr = TSHIM_NEW(FILE)();
        TSHIM_H2T(FILE)(&tr, &hr);
    }
    return tr;
}

TSHIM(FILE) *SHIM(fopen64)(const char *a, const char *b)
{
    FILE *hr;
    TSHIM(FILE) *tr = NULL;
    hr = fopen(a, b);
    if (hr) {
        tr = TSHIM_NEW(FILE)();
        TSHIM_H2T(FILE)(&tr, &hr);
    }
    return tr;
}

size_t SHIM(fread)(void *a, size_t b, size_t c, TSHIM(FILE) *d)
{
    FILE *hd;
    TSHIM_T2H(FILE)(&hd, &d);
    return fread(a, b, c, hd);
}
