#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shim.h"

void SHIM(_Jv_RegisterClasses)() {}
void SHIM(__ctype)() {}
void SHIM(__fpstart)() {}
UNIMPL_SHIM(_get_exit_frame_monitor);
UNIMPL_SHIM(__flsbuf);

/* Solaris puts std* in __iob, and has the files directly there so we basically
 * just have to hope we get lucky */
#define SOLARIS_FILE_SZ 48
struct notQuiteAFile {
    char buf[SOLARIS_FILE_SZ];
};
struct notQuiteAFile SHIM(__iob)[4];

CONSTRUCTOR(void shim_init__iob())
{
    memcpy(SHIM(__iob), stdin, SOLARIS_FILE_SZ);
    memcpy(SHIM(__iob) + 1, stdout, SOLARIS_FILE_SZ);
    memcpy(SHIM(__iob) + 2, stderr, SOLARIS_FILE_SZ);
    memset(SHIM(__iob) + 3, 0, SOLARIS_FILE_SZ);
}
