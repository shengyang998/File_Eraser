all:
	mkdir release 2>/dev/null; g++ *.cpp -o release/eraser -O2 -std=c++0x;

install:
	cp release/eraser /usr/local/bin/eraser

