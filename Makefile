

all: clean test

test: test.cpp cppopts.h Makefile
	g++ test.cpp -o test
	./test.sh

test2: test2.cpp cppopts.h
	g++ test2.cpp -o test2
	./test2

clean: 
	rm -f test
