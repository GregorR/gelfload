bin_PROGRAMS = microcosm-@GELFLOAD_ARCH@ microcosm-ld-@GELFLOAD_ARCH@
noinst_DATA = microcosm$(EXEEXT)
lib_LIBRARIES = libgelfload.a

microcosm_@GELFLOAD_ARCH@_SOURCES = main.c whereami.c
microcosm_@GELFLOAD_ARCH@_DEPENDENCIES = libgelfload.a
microcosm_@GELFLOAD_ARCH@_LDADD = libgelfload.a

if HAVE_LIBDL
    microcosm_@GELFLOAD_ARCH@_LDADD += -ldl
endif

microcosm_ld_@GELFLOAD_ARCH@_SOURCES = elfload-ld.c whereami.c
if HAVE_STATIC
    microcosm_ld_@GELFLOAD_ARCH@_LDFLAGS = -static
endif

libgelfload_a_SOURCES = bbuffer.c elfload.c dlfcn.c microcosm.c

microcosm$(EXEEXT): microcosm-@GELFLOAD_ARCH@$(EXEEXT)
	-rm -f microcosm$(EXEEXT)
	$(LN_S) microcosm-@GELFLOAD_ARCH@$(EXEEXT) microcosm$(EXEEXT)

install-exec-hook:
	cd $(DESTDIR)$(bindir) && \
	    rm -f microcosm$(EXEEXT) && \
	    $(LN_S) microcosm-@GELFLOAD_ARCH@$(EXEEXT) microcosm$(EXEEXT)
