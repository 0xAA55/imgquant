CXXFLAGS=-I. -I.. -std=c++20 -O3 -flto

all: quant

quant: imgquant/imgquant
	cp imgquant/imgquant quant

imgquant/imgquant:
	make -C imgquant

clean:
	rm -f quant
	make -C imgquant clean
	make -C pngcpp clean
	make -C rgbto8 clean
