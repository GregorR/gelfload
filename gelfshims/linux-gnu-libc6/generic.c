#include <stdio.h>
#include "shim.h"

UNIMPL_SHIM(__errno_location);
UNIMPL_SHIM(__ctype_get_mb_cur_max);
UNIMPL_SHIM(bindtextdomain);
UNIMPL_SHIM(__overflow);
UNIMPL_SHIM(dcgettext);
UNIMPL_SHIM(__ctype_b_loc);
UNIMPL_SHIM(error);
UNIMPL_SHIM(textdomain);
