#define _SVID_SOURCE /* for strdup */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shim.h"

int SHIM(bindtextdomain)() { return 0; }
int SHIM(textdomain)() { return 0; }
int SHIM(open64)(const char *path, int oflag, int mode) { return open(path, oflag, mode); }
void SHIM(__assert_fail)() { abort(); }
char *SHIM(__strdup)(const char *a) { return strdup(a); }

UNIMPL_SHIM(__ctype_get_mb_cur_max);
UNIMPL_SHIM(__ctype_b_loc);
UNIMPL_SHIM(error);
UNIMPL_SHIM(__fxstat64);
UNIMPL_SHIM(dirfd);
UNIMPL_SHIM(__xstat64);
UNIMPL_SHIM(fchownat);
UNIMPL_SHIM(__lxstat64);
UNIMPL_SHIM(openat64);
UNIMPL_SHIM(readdir64);
UNIMPL_SHIM(__fxstatat64);
UNIMPL_SHIM(fdopendir);
