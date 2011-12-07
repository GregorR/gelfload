#include <setjmp.h>
#include <stdio.h>
#include "shim.h"

void SHIM(__ctype_b)() {}
void SHIM(__libc_init)() {}
void SHIM(__setfpucw)() {}
void SHIM(__setjmp)(jmp_buf env) { return setjmp(env); }
