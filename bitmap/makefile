CXXFLAGS=-std=c++20 -O3 -flto

all: libbitmap.a

libbitmap.a: coldef.o bitmap.o
	$(AR) rcs $@ $^

clean:
	rm -f *.a *.o
