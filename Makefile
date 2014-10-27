

all: test3


# all: clean test

test: test.cpp cppopts.h Makefile
	g++ -ggdb test.cpp -o test
	./test.sh

test2: test2.cpp cppopts.h
	g++ test2.cpp -o test2
	./test2

test3: test3.cpp commandline.h Makefile
	g++ -g test3.cpp -o test3
	# gdb test3
	./test3 x 4 y 3 string "this is a string"

clean: 
	rm -f test test2 test3
