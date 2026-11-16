
all: $(OBJS)
	make -f Makefile -C src
	cp */*.a .
	cp src/glide.h src/sdk2_*.h .
	make -f Makefile -C test
	make -f Makefile -C gldemo
	make -f Makefile -C nostalgic-glide-demo

clean:
	make -f Makefile -C src clean
	make -f Makefile -C test clean
	make -f Makefile -C gldemo clean
	make -f Makefile -C nostalgic-glide-demo clean
	rm -f *.a *.h
