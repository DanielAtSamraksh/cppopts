

all: clean test

test: test.cpp argv2obj.h Makefile
	g++ test.cpp -o test
	./test -S'Short String' -bi2 -s string --string=aString -f 3.4 --bool=false --chars=charstring --float=3.24 --int 5 -i6

clean: 
	rm -f test
