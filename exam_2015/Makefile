include Makefile.def
.PHONY: all test clean

all:
	cd src/; make

test:
	cd src/; make clean
	cd src/; make
	cd test/; make

clean:
	cd src/; make clean
	cd test/; make clean
