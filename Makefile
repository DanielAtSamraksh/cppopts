

all: clean test

test: test.cpp argv2obj.h Makefile
	g++ test.cpp -o test

	./test -b
	./test --bool=false
	./test --bool=true

	./test -s'Short String'
	./test --chars='Short String'

	./test -s 'Short String'
	./test --chars 'Short String'

	./test -bi2 -s string -f 3.4
	./test --string str --bool=false --chars=charstring --float=3.24 --int 5 -i6
	./test --string str --bool=false --chars=charstring --float=3.24 --int 5 
	./test --string=str --bool=false --chars=charstring -f3.24 
	./test --string=str --bool=false -- --chars=charstring -f3.24 
	./test --string str --bool=false --f --chars=charstring --float=3.24 --int=5
	./test --string str --bool=false --chars=charstring --float=3.24 --int=5

clean: 
	rm -f test
