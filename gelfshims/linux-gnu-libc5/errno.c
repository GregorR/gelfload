#include <stdio.h>
#include <stdlib.h>
#include "shim.h"

/* oh well, you're not going to get a very good errno! */
static int fake_errno = 0;
int *SHIM(_errno) = &fake_errno;
