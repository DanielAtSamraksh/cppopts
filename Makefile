

all: clean test

test: test.cpp cppopts.h Makefile
	g++ test.cpp -o test
	./test.sh

clean: 
	rm -f test
