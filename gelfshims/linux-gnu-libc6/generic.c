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
int SHIM(__ctype_get_mb_cur_max)() { return MB_CUR_MAX; }

UNIMPL_SHIM(__ctype_b_loc);
UNIMPL_SHIM(error);
UNIMPL_SHIM(dirfd);
UNIMPL_SHIM(fchownat);
UNIMPL_SHIM(__lxstat64);
UNIMPL_SHIM(openat64);
UNIMPL_SHIM(readdir64);
UNIMPL_SHIM(__fxstatat64);
UNIMPL_SHIM(fdopendir);
UNIMPL_SHIM(__getdelim);
UNIMPL_SHIM(mempcpy);
UNIMPL_SHIM(fseeko64);
UNIMPL_SHIM(__freading);
UNIMPL_SHIM(__xmknod);
UNIMPL_SHIM(readlinkat);
UNIMPL_SHIM(euidaccess);
UNIMPL_SHIM(rpmatch);
UNIMPL_SHIM(utimensat);
UNIMPL_SHIM(ftruncate64);
UNIMPL_SHIM(futimesat);
UNIMPL_SHIM(linkat);
UNIMPL_SHIM(error_at_line);
UNIMPL_SHIM(futimens);
UNIMPL_SHIM(lseek64);
UNIMPL_SHIM(_Jv_RegisterClasses);
UNIMPL_SHIM(alphasort64);
UNIMPL_SHIM(__fsetlocking);
UNIMPL_SHIM(umount2);
UNIMPL_SHIM(umount);
UNIMPL_SHIM(fgets_unlocked);
UNIMPL_SHIM(strverscmp);
UNIMPL_SHIM(__strtok_r);
UNIMPL_SHIM(___tls_get_addr);
UNIMPL_SHIM(mkstemp64);
UNIMPL_SHIM(scandir64);
UNIMPL_SHIM(mmap64);
UNIMPL_SHIM(__rawmemchr);
UNIMPL_SHIM(getrlimit64);
UNIMPL_SHIM(__strndup);
UNIMPL_SHIM(__ctype_toupper_loc);
UNIMPL_SHIM(__ctype_tolower_loc);
UNIMPL_SHIM(clock_settime);
