
#CC = m68k-apple-macos-g++
#CXX = m68k-apple-macos-g++
#CC = powerpc-apple-macos-gcc
#CXX = powerpc-apple-macos-gcc


CFLAGS=-I.. -I/opt/local/include -I..
#CFLAGS=-I..

LDFLAGS=../.libs/libglide2x.a -L/opt/local/lib -framework OpenGL
#LDFLAGS=-L.. -lglide2x -lOpenGlLibrary -lOpenGLUtility -lm

SDLLIBS=-lSDLmain -lSDL -framework Cocoa -framework Carbon -framework IOKit -framework CoreVideo -framework AudioToolbox -framework ForceFeedback
#SDLLIBS=-lSDL

SDLMAIN=
#SDLMAIN=$(HOME)/SDL-1.2-main/obj/SDL_main.c.o


all: sdl1glide2 glide2 test1glide2 test2glide2 test3glide2


sdl1glide2:	sdl1glide2.o
	$(CXX) -o sdl1glide2 $(SDLMAIN) sdl1glide2.o $(LDFLAGS) $(SDLLIBS)

sdl2glide2:	sdl2glide2.o
	$(CXX) -o sdl2glide2 sdl2glide2.o $(LDFLAGS) -lSDL2main -lSDL2

sdl3glide2:	sdl3glide2.o
	$(CXX) -o sdl3glide2 sdl3glide2.o $(LDFLAGS) -lSDL3


glide2:	glide2.o
	$(CXX) -o glide2 glide2.o $(LDFLAGS) $(SDLLIBS) 

glide2test:	glide2test.o
	$(CXX) -o glide2test glide2test.o $(LDFLAGS) $(SDLLIBS) 


test1glide2:	test1glide2.o
	$(CXX) -o test1glide2 test1glide2.o $(LDFLAGS) $(SDLLIBS) 

test2glide2:	test2glide2.o
	$(CXX) -o test2glide2 test2glide2.o $(LDFLAGS) $(SDLLIBS) 

test3glide2:	test3glide2.o
	$(CXX) -o test3glide2 test3glide2.o $(LDFLAGS) $(SDLLIBS) 


glide3:	glide3.o
	$(CXX) -o glide3 glide3.o $(LDFLAGS) -lSDL3
	

clean:
	rm -f *.o glide2 glide2test glide3 sdl1glide2 sdl2glide2 sdl3glide2 test1glde2 test2glde2 test3glde2
	rm -f *.bin *.APPL *.dsk *.gdb *.xcoff *.pef OpenGLid.*
