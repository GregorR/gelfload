#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.6)
AC_INIT([gelfload-host-core], [0.1], [cdgregorr@users.sf.net])
AC_CONFIG_SRCDIR([core.c])
AC_CONFIG_HEADER([config.h])

AC_CONFIG_AUX_DIR([../../scripts])
AM_INIT_AUTOMAKE()

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
AC_CHECK_LIB([iconv], [iconv])
AC_CHECK_LIB([rt], [clock_gettime])

for ldlib in /lib/ld*.so*; do
    LD_LIBS="$LIBS"
    LIBS="$LIBS $ldlib"; export LIBS
    AC_CHECK_LIB([c], [open], [true], [
        LIBS="$LD_LIBS"; export LIBS
    ])
done

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
