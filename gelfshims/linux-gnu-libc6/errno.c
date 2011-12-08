#include <errno.h>
#include "shim.h"

int *SHIM(__errno_location)() { return &errno; }
