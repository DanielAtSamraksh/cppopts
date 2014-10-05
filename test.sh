#!/bin/bash
set -x


# ./test -b

# ./test --bool=false
# ./test --bool=true

# ./test -s'Short String'
# ./test --chars='Short String'

# ./test -s 'Short String'
# ./test --chars 'Short String'

# ./test -bi2 -s string -f 3.4
# ./test --string str --bool=false --chars=charstring --float=3.24 --int 5 -i6
# ./test --string str --bool=false --chars=charstring --float=3.24 --int 5 
# ./test --string=str --bool=false --chars=charstring -f3.24 
# ./test --string=str --bool=false -- --chars=charstring -f3.24 
# ./test --string str --bool=false --f --chars=charstring --float=3.24 --int=5
# ./test --string str --bool=false --chars=charstring --float=3.24 --int=5


./test -b | diff -Bbqy - <(cat - <<EOF
myBool= (bool) 1
myInt= (int) 0
myFloat= (double) 0
myString= (string) 
myChars= (char*) 

myBool = 1
myInt = 0
myFloat = 0.000000
myString = 
myChars = 
EOF
) && echo ok

./test --bool=false | diff -Bbqy - <(cat - <<EOF
myBool= (bool) 0
myInt= (int) 0
myFloat= (double) 0
myString= (string) 
myChars= (char*) 

myBool = 0
myInt = 0
myFloat = 0.000000
myString = 
myChars = 


EOF
) && echo ok

./test --bool=true | diff -Bbqy - <(cat - <<EOF
myBool= (bool) 1
myInt= (int) 0
myFloat= (double) 0
myString= (string) 
myChars= (char*) 

myBool = 1
myInt = 0
myFloat = 0.000000
myString = 
myChars = 


EOF
) && echo ok

./test '-sShort String' | diff -Bbqy - <(cat - <<EOF
myBool= (bool) 0
myInt= (int) 0
myFloat= (double) 0
myString= (string) 
myChars= (char*) Short String

myBool = 0
myInt = 0
myFloat = 0.000000
myString = 
myChars = Short String


EOF
) && echo ok

./test '--chars=Short String' | diff -Bbqy - <(cat - <<EOF
myBool= (bool) 0
myInt= (int) 0
myFloat= (double) 0
myString= (string) 
myChars= (char*) Short String

myBool = 0
myInt = 0
myFloat = 0.000000
myString = 
myChars = Short String


EOF
) && echo ok

./test -s 'Short String' | diff -Bbqy - <(cat - <<EOF
myBool= (bool) 0
myInt= (int) 0
myFloat= (double) 0
myString= (string) 
myChars= (char*) Short String

myBool = 0
myInt = 0
myFloat = 0.000000
myString = 
myChars = Short String


EOF
) && echo ok

./test --chars 'Short String' | diff -Bbqy - <(cat - <<EOF
myBool= (bool) 0
myInt= (int) 0
myFloat= (double) 0
myString= (string) 
myChars= (char*) Short String

myBool = 0
myInt = 0
myFloat = 0.000000
myString = 
myChars = Short String


EOF
) && echo ok

./test -bi2 -s string -f 3.4 | diff -Bbqy - <(cat - <<EOF
myBool= (bool) 1
myInt= (int) 2
myFloat= (double) 3.4
myString= (string) 
myChars= (char*) string

myBool = 1
myInt = 2
myFloat = 3.400000
myString = 
myChars = string


EOF
) && echo ok

./test --string str --bool=false --chars=charstring --float=3.24 --int 5 -i6 | diff -Bbqy - <(cat - <<EOF
myBool= (bool) 0
myInt= (int) 6
myFloat= (double) 3.24
myString= (string) str
myChars= (char*) charstring

myBool = 0
myInt = 6
myFloat = 3.240000
myString = str
myChars = charstring


EOF
) && echo ok

./test --string str --bool=false --chars=charstring --float=3.24 --int 5 | diff -Bbqy - <(cat - <<EOF
myBool= (bool) 0
myInt= (int) 5
myFloat= (double) 3.24
myString= (string) str
myChars= (char*) charstring

myBool = 0
myInt = 5
myFloat = 3.240000
myString = str
myChars = charstring


EOF
) && echo ok

./test --string=str --bool=false --chars=charstring -f3.24 | diff -Bbqy - <(cat - <<EOF
myBool= (bool) 0
myInt= (int) 0
myFloat= (double) 3.24
myString= (string) str
myChars= (char*) charstring

myBool = 0
myInt = 0
myFloat = 3.240000
myString = str
myChars = charstring


EOF
) && echo ok

./test --string=str --bool=false -- --chars=charstring -f3.24 | diff -Bbqy - <(cat - <<EOF
myBool= (bool) 0
myInt= (int) 0
myFloat= (double) 0
myString= (string) str
myChars= (char*) 
0=--chars=charstring
1=-f3.24

myBool = 0
myInt = 0
myFloat = 0.000000
myString = str
myChars = 


EOF
) && echo ok

./test --string str --bool=false --f --chars=charstring --float=3.24 --int=5 | diff -Bbqy - <(cat - <<EOF
myBool= (bool) 0
myInt= (int) 5
myFloat= (double) 3.24
myString= (string) str
myChars= (char*) charstring
unknown option 0=--f

myBool = 0
myInt = 5
myFloat = 3.240000
myString = str
myChars = charstring


EOF
) && echo ok

./test --string str --bool=false --chars=charstring --float=3.24 --int=5 | diff -Bbqy - <(cat - <<EOF
myBool= (bool) 0
myInt= (int) 5
myFloat= (double) 3.24
myString= (string) str
myChars= (char*) charstring

myBool = 0
myInt = 5
myFloat = 3.240000
myString = str
myChars = charstring

EOF
) && echo ok

