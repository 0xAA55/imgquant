CXXFLAGS=-I.. -std=c++20 -O3 -flto
LDFLAGS=-L../pngcpp -L../rgbto8
LDLIBS=-lstdc++ -lpng -lpngcpp -lrgbto8
OBJS=main.o

imgquant: $(OBJS) ../pngcpp/libpngcpp.a ../rgbto8/librgbto8.a
	$(CC) $(OBJS) -o $@ $(LDLIBS) $(LDFLAGS)

../pngcpp/libpngcpp.a:
	make -C ../pngcpp

../rgbto8/librgbto8.a:
	make -C ../rgbto8

clean:
	rm -f *.o imgquant
