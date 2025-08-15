CXXFLAGS=-I.. -std=c++20 -O3 -flto

all: libpngcpp.a

libpngcpp.a: pngcpp.o
	$(AR) rcs $@ $^

clean:
	rm -f *.a *.o
