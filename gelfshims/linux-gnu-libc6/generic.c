#include <stdio.h>
#include "shim.h"

int SHIM(bindtextdomain)() { return 0; }
int SHIM(textdomain)() { return 0; }
UNIMPL_SHIM(__errno_location);
UNIMPL_SHIM(__ctype_get_mb_cur_max);
UNIMPL_SHIM(dcgettext);
UNIMPL_SHIM(__ctype_b_loc);
UNIMPL_SHIM(error);
