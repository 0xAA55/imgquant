CXXFLAGS=-I.. -std=c++20 -O3 -flto
LDFLAGS=-L../pngcpp -L../rgb24to8
LDLIBS=-lstdc++ -lpng -lpngcpp -lrgb24to8
OBJS=main.o

imgquant: $(OBJS) ../pngcpp/libpngcpp.a ../rgb24to8/librgb24to8.a
	$(CC) $(OBJS) -o $@ $(LDLIBS) $(LDFLAGS)

../pngcpp/libpngcpp.a:
	make -C ../pngcpp

../rgb24to8/librgb24to8.a:
	make -C ../rgb24to8

clean:
	rm -f *.o imgquant
