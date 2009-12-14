CC=gcc
LD=ld
CFLAGS=
LDFLAGS=--dynamic-linker=/usr/bin/gelfload-ld
SOFLAGS=-shared -soname=

TESTSTATIC_OBJS=teststatic.o
TESTHOST_OBJS=testhost.o
TESTSO_OBJS=testso.o
TESTDL_OBJS=testdl.o
LIBC_OBJS=libc_wrap.o

all: teststatic testhost testso.so testdl libc.so

teststatic: $(TESTSTATIC_OBJS)
	$(LD) $(LDFLAGS) $(TESTSTATIC_OBJS) -o teststatic

testhost: $(TESTHOST_OBJS) testso.so
	$(LD) $(LDFLAGS) $(TESTHOST_OBJS) testso.so -o testhost

testso.so: $(TESTSO_OBJS)
	$(LD) $(LDFLAGS) $(TESTSO_OBJS) $(SOFLAGS)testso.so -o testso.so

testdl: libc.so $(TESTDL_OBJS)
	$(LD) $(LDFLAGS) $(TESTDL_OBJS) libc.so -o testdl

libc.so: $(LIBC_OBJS)
	$(LD) $(LDFLAGS) $(LIBC_OBJS) $(SOFLAGS)libhost_.so -o libc.so

.s.o:
	$(CC) $(CFLAGS) -c $< -o $@

.c.o:
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

clean:
	rm -f teststatic $(TESTSTATIC_OBJS)
	rm -f testhost $(TESTHOST_OBJS)
	rm -f testso.so $(TESTSO_OBJS)
	rm -f testdl $(TESTDL_OBJS)
	rm -f libc.so $(LIBC_OBJS)
