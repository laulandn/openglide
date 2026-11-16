#PATH_TO_RETRO68=$(HOME)/Retro68-build/toolchain/powerpc-apple-macos

#CC=m68k-apple-macos-g++
#CXX=m68k-apple-macos-g++
#AR=m68k-apple-macos-ar
#RANLIB=m68k-apple-macos-ranlib
#CC=powerpc-apple-macos-gcc
#CXX=powerpc-apple-macos-g++
#AR=powerpc-apple-macos-ar
RANLIB=ranlib

#CFLAGS= -I. -I../.. -I$(HOME)/sdl12-compat/include/SDL -I$(HOME)/Mesa-2.4.1/Headers -DFxU64=long -DC_USE_SDL
CFLAGS= -I. -I../.. -I/opt/local/include -DFxU64=long -DC_USE_SDL

OBJS=$(subst .c,.o,$(wildcard *.c)) $(subst .cpp,.o,$(wildcard *.cpp))

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

.cpp.o:
	$(CXX) $(CFLAGS) -c -o $@ $<


all: $(OBJS)

clean:
	rm -f *.o *.lo
