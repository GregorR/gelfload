#include <stdio.h>
#include "shim.h"

#include "redirect.h"

#ifndef REDIRECT_ATEXIT
DIRECT_INT_FUNC(atexit);
#endif
