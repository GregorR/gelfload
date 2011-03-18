CC=gcc
ECFLAGS=
CFLAGS=-O3 -g -shared -fPIC $(ECFLAGS)
LD=$(CC)
ELDFLAGS=
LDFLAGS=$(ELDFLAGS)

all: ../lib/gelfload/libgelfload-generishims.so.0

../lib/gelfload/libgelfload-generishims.so.0: generishims.c
	mkdir -p ../lib/gelfload
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

clean:
	rm -f ../lib/gelfload/libgelfload-generishims.so.0
