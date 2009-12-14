#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.5)
AC_INIT([gelfload], [1], [cdgregorr@sourceforge.net])
AC_CONFIG_SRCDIR([src/elfload.h])
AC_CONFIG_HEADER([config.h])

AC_CONFIG_AUX_DIR([scripts])
AM_INIT_AUTOMAKE()

m4_include([scripts/ac_compile_check_sizeof.ac])

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
AC_LINK_IFELSE(AC_LANG_PROGRAM([], []), [], [
    # OK, try with --section-start. This test comes second because MingW allows
    # it, though it doesn't work
    LDFLAGS="$ORIG_LDFLAGS -Wl,--section-start,.dynsym=0x18000000"
    export LDFLAGS
    AC_LINK_IFELSE(AC_LANG_PROGRAM([], []), [], [
        # Too bad, this can't be done
        LDFLAGS="$ORIG_LDFLAGS"
        export LDFLAGS
    ])
])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([dlfcn.h fcntl.h stdint.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_COMPILE_CHECK_SIZEOF([void *])

# Checks for library functions.
#AC_FUNC_ALLOCA
#AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_FUNC_MMAP
AC_CHECK_FUNCS([memset strdup])

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
