

all: test

test: test.cpp argv2obj.h
	g++ test.cpp -o test
	./test -n 2 -i
	./test -n 2
	./test -i
	./test
	./test -n 2 -i -3

clean: 
	rm -f test
