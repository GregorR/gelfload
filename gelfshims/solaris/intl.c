#include <libintl.h>
#include "shim.h"

/* Solaris prefixes these with libintl_ */
char *SHIM(libintl_bindtextdomain)(const char *a, const char *b) { return bindtextdomain(a, b); }
char *SHIM(libintl_gettext)(const char *a) { return gettext(a); }
char *SHIM(libintl_textdomain)(const char *a) { return textdomain(a); }
