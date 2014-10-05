#!/bin/bash
# set -x


./test -b | diff -Bby --suppress-common-lines - <(cat - <<EOF
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
) && echo ok || echo fail

./test --bool=false | diff -Bby --suppress-common-lines - <(cat - <<EOF
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
) && echo ok || echo fail

./test --bool=true | diff -Bby --suppress-common-lines - <(cat - <<EOF
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
) && echo ok || echo fail

./test -b -s'Short String' | diff -Bby --suppress-common-lines - <(cat - <<EOF
myBool= (bool) 1
myInt= (int) 0
myFloat= (double) 0
myString= (string) 
myChars= (char*) Short String

myBool = 1
myInt = 0
myFloat = 0.000000
myString = 
myChars = Short String


EOF
) && echo ok || echo fail

./test '--chars=Short String' | diff -Bby --suppress-common-lines - <(cat - <<EOF
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
) && echo ok || echo fail

./test -s 'Short String' | diff -Bby --suppress-common-lines - <(cat - <<EOF
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
) && echo ok || echo fail

./test --chars 'Short String' | diff -Bby --suppress-common-lines - <(cat - <<EOF
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
) && echo ok || echo fail

./test -bi2 -s string -f 3.4 | diff -Bby --suppress-common-lines - <(cat - <<EOF
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
) && echo ok || echo fail

./test --string str --bool=false --chars=charstring --float=3.24 --int 5 -i6 | diff -Bby --suppress-common-lines - <(cat - <<EOF
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
) && echo ok || echo fail

./test --string str --bool=false --chars=charstring --float=3.24 --int 5 | diff -Bby --suppress-common-lines - <(cat - <<EOF
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
) && echo ok || echo fail

./test --string=str --bool=false --chars=charstring -f3.24 | diff -Bby --suppress-common-lines - <(cat - <<EOF
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
) && echo ok || echo fail

./test --string=str --bool=false -- --chars=charstring -f3.24 | diff -Bby --suppress-common-lines - <(cat - <<EOF
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
) && echo ok || echo fail

./test --string str --bool=false --f --chars=charstring --float=3.24 --int=5 | diff -Bby --suppress-common-lines - <(cat - <<EOF
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
) && echo ok || echo fail

./test --string str --bool=false --chars=charstring --float=3.24 --int=5 | diff -Bby --suppress-common-lines - <(cat - <<EOF
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
) && echo ok || echo fail

