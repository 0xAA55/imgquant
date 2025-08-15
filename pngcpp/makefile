CXXFLAGS=-I.. -std=c++20 -O3 -flto -ffat-lto-objects -fopenmp

all: libpngcpp.a

libpngcpp.a: pngcpp.o
	$(AR) rcs $@ $^

clean:
	rm -f *.a *.o
