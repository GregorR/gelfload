#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.50)
AC_INIT([gelfload], [1], [cdgregorr@sourceforge.net])
AC_CONFIG_SRCDIR([src/elfload.h])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_AUX_DIR([scripts])

m4_include([scripts/lean.m4])
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CC
AC_PROG_RANLIB

# Checks for libraries.
AC_CHECK_LIB([dl], [dlopen], [
    AC_DEFINE([HAVE_LIBDL], 1, [Defined if libdl is available.])
    HAVE_LIBDL=true
], [
    HAVE_LIBDL=false
])
AM_CONDITIONAL(HAVE_LIBDL, test "x$HAVE_LIBDL" = "xtrue")

# Check if we can rebase the program at 0x18000000
ORIG_LDFLAGS="$LDFLAGS"
LDFLAGS="$LDFLAGS -Wl,--image-base,0x18000000"
export LDFLAGS
AC_LINK_IFELSE([AC_LANG_PROGRAM([], [])], [], [
    # OK, try with --section-start. This test comes second because MingW allows
    # it, though it doesn't work
    LDFLAGS="$ORIG_LDFLAGS -Wl,--section-start,.dynsym=0x19000000 -Wl,--section-start,.hash=0x18000000"
    export LDFLAGS
    AC_LINK_IFELSE([AC_LANG_PROGRAM([], [])], [], [
        # Maybe we're using a Solaris-style linker, let's try a map file
        srcdir_absolute=`cd "$srcdir" && pwd`
        LDFLAGS="$ORIG_LDFLAGS -Wl,-M$srcdir_absolute/scripts/0x18000000.solaris.map"
        export LDFLAGS
        AC_LINK_IFELSE([AC_LANG_PROGRAM([], [])], [], [
            # Too bad, this can't be done
            LDFLAGS="$ORIG_LDFLAGS"
            export LDFLAGS
        ])
    ])
])

# Checks for header files.
AC_CHECK_HEADERS([dlfcn.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_SIZEOF([void *])

# -static
ORIG_LDFLAGS="$LDFLAGS"
LDFLAGS="$LDFLAGS -static"
AC_LINK_IFELSE([AC_LANG_PROGRAM([], [])], [
    HAVE_STATIC=yes
], [
    HAVE_STATIC=no
])
LDFLAGS="$ORIG_LDFLAGS"
AM_CONDITIONAL(HAVE_STATIC, test x$HAVE_STATIC = xyes)

# Checks for library functions.
AC_CHECK_FUNCS([mmap])

# Prefix for elfload-ld.c to know where the main program is:
AC_DEFINE_UNQUOTED([PREFIX], ["$prefix"], [Install prefix])

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
