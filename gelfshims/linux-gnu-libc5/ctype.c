#include <ctype.h>
#include "shim.h"

int SHIM(__ctype_tolower)(int a) { return tolower(a); }
int SHIM(__ctype_toupper)(int a) { return toupper(a); }
