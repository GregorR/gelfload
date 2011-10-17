#include <stdio.h>
#include "shim.h"

void SHIM(__ctype_b)() {}
void SHIM(__libc_init)() {}
void SHIM(__setfpucw)() {}
