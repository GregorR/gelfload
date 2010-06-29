bin_PROGRAMS = microcosm microcosm-ld
noinst_LIBRARIES = libgelfload.a
noinst_HEADERS = elfload.h elfload_elf.h elfload_exec.h

microcosm_SOURCES = main.c
microcosm_DEPENDENCIES = libgelfload.a
microcosm_LDADD = libgelfload.a

microcosm_ld_SOURCES = elfload-ld.c
microcosm_ld_LDFLAGS = -static

if HAVE_LIBDL
    microcosm_LDADD += -ldl
endif

libgelfload_a_SOURCES = bbuffer.c elfload.c dlfcn.c microcosm.c
