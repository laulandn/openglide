LIBNAME=glide2x

#PATH_TO_RETRO68=$(HOME)/Retro68-build/toolchain/m68k-apple-macos

#CC=m68k-apple-macos-gcc
#CXX=m68k-apple-macos-g++
#AR=m68k-apple-macos-ar
#RANLIB=m68k-apple-macos-ranlib
#CC=powerpc-apple-macos-gcc
#CXX=powerpc-apple-macos-g++
#AR=powerpc-apple-macos-ar
RANLIB=ranlib

CFLAGS= -I. -Iplatform/linux -I/opt/local/include -I/opt/local/include/GL -DFxU64="unsigned long long"


OBJS=$(subst .c,.o,$(wildcard *.c)) $(subst .cpp,.o,$(wildcard *.cpp))


.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

.cpp.o:
	$(CXX) $(CFLAGS) -c -o $@ $<


all: $(OBJS)
	make -f Makefile -C platform
	$(AR) rcv lib$(LIBNAME).a *.o */*/*.o
	$(RANLIB) lib$(LIBNAME).a

clean:
	make -f Makefile -C platform clean
	rm -f *.o */*.o */*.*.o *.a *.lo
