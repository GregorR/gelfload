bin_PROGRAMS = gelfload gelfload-ld
lib_LIBRARIES = libgelfload.a
include_HEADERS = elfload.h elfload_elf.h elfload_exec.h

gelfload_SOURCES = main.c whereami.c
gelfload_DEPENDENCIES = libgelfload.a
gelfload_LDADD = libgelfload.a

gelfload_ld_SOURCES = elfload-ld.c whereami.c
if HAVE_STATIC
    gelfload_ld_LDFLAGS = -static
endif

if HAVE_LIBDL
    gelfload_LDADD += -ldl
endif

libgelfload_a_SOURCES = bbuffer.c elfload.c dlfcn.c
