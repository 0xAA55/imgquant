CXXFLAGS=-I.. -std=c++20 -O3 -flto -ffat-lto-objects -fopenmp
LDFLAGS=-L../bitmap -L../pngcpp -L../rgbto8
LDLIBS=-lstdc++ -lpng -lbitmap -lpngcpp -lrgbto8 -lgomp
OBJS=main.o

imgquant: $(OBJS) ../bitmap/libbitmap.a ../pngcpp/libpngcpp.a ../rgbto8/librgbto8.a
	$(CC) $(OBJS) -o $@ $(LDLIBS) $(LDFLAGS)

../bitmap/libbitmap.a:
	make -C ../bitmap

../pngcpp/libpngcpp.a:
	make -C ../pngcpp

../rgbto8/librgbto8.a:
	make -C ../rgbto8

clean:
	rm -f *.o imgquant
