CXXFLAGS=-I.. -std=c++20 -O3 -flto -ffat-lto-objects -fopenmp

all: librgbto8.a

librgbto8.a: octree.o dither.o rgb2i.o rgb24to8.o rgb32to8.o
	$(AR) rcs $@ $^

clean:
	rm -f *.a *.o
