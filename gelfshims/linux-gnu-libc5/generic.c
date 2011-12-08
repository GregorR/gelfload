#include <stdio.h>
#include "shim.h"

/* FIXME: I don't really know what this is */
unsigned char SHIM(__ctype_b)[32];

void SHIM(__libc_init)() {}
void SHIM(__setfpucw)() {}
