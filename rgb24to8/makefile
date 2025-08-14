CXXFLAGS=-I.. -std=c++20 -O3 -flto

all: librgb24to8.a

librgb24to8.a: octree.o dither.o cubemapper.o rgb24to8.o
	$(AR) rcs $@ $^

clean:
	rm -f *.a *.o
