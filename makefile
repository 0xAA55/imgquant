CXXFLAGS=-I. -I.. -std=c++20 -O3 -flto -fopenmp

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
	make -C bitmap clean

test1: quant test1.png
	./quant test1.png test1out.png

test2: quant test2.png
	./quant test2.png test2out.png

test3: quant test3.png
	./quant test3.png test3out.png

test4: quant test4.png
	./quant test4.png test4out.png

tests: test1 test2 test3 test4
