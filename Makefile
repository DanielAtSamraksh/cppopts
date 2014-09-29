

all: test

test: test.cpp argv2obj.h Makefile
	g++ test.cpp -o test
	./test -n 2 -i
	./test -n 2
	./test -i
	./test
	./test -n 2 -i -3
	./test --long-bool=5 
	./test --long-bool=1 
	./test --long-bool=false -n 5 -ifalse

clean: 
	rm -f test
