#include <stdio.h>
#include "shim.h"

void SHIM(_Jv_RegisterClasses)() {}
void SHIM(__ctype)() {}
void SHIM(__fpstart)() {}
UNIMPL_SHIM(_get_exit_frame_monitor);
UNIMPL_SHIM(__filbuf);
UNIMPL_SHIM(__ctype_mask);
UNIMPL_SHIM(_tzname);
UNIMPL_SHIM(__minor);
UNIMPL_SHIM(acl);
UNIMPL_SHIM(libintl_dcgettext);
UNIMPL_SHIM(__major);

/* this has to be redirected for rather silly reasons */
int SHIM(atexit)(void (*a)(void))
{
    static int c = 0;
    if (c >= 3) {
        return atexit(a);
    } else {
        c++;
        return 0;
    }
}
