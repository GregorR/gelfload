bin_PROGRAMS = gelfload-@GELFLOAD_ARCH@ gelfload-ld-@GELFLOAD_ARCH@
noinst_DATA = gelfload$(EXEEXT)
lib_LIBRARIES = libgelfload.a

gelfload_@GELFLOAD_ARCH@_SOURCES = main.c whereami.c
gelfload_@GELFLOAD_ARCH@_DEPENDENCIES = libgelfload.a
gelfload_@GELFLOAD_ARCH@_LDADD = libgelfload.a

if HAVE_LIBDL
    gelfload_@GELFLOAD_ARCH@_LDADD += -ldl
endif

gelfload_ld_@GELFLOAD_ARCH@_SOURCES = elfload-ld.c whereami.c
if HAVE_STATIC
    gelfload_ld_@GELFLOAD_ARCH@_LDFLAGS = -static
endif

libgelfload_a_SOURCES = bbuffer.c elfload.c dlfcn.c

gelfload$(EXEEXT): gelfload-@GELFLOAD_ARCH@$(EXEEXT)
	-rm -f gelfload$(EXEEXT)
	$(LN_S) gelfload-@GELFLOAD_ARCH@$(EXEEXT) gelfload$(EXEEXT)

install-exec-hook:
	cd $(DESTDIR)$(bindir) && \
	    rm -f gelfload$(EXEEXT) && \
	    $(LN_S) gelfload-@GELFLOAD_ARCH@$(EXEEXT) gelfload$(EXEEXT)
