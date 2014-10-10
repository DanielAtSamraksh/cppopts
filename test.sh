#!/bin/bash
# set -x


./test -b | diff -Bby --suppress-common-lines - <(cat - <<EOF
myBool= (bool) 1
myInt= (int) 0
myFloat= (double) 0
myString= (string) 
myChars= (char*) 
myNum= (string) zero
myChoices= (vector<string>) []
myChoice= (string)

myBool = 1
myInt = 0
myFloat = 0.000000
myString = 
myChars = 
myNum = zero
EOF
) && echo ok || echo fail

./test --bool=false | diff -Bby --suppress-common-lines - <(cat - <<EOF
myBool= (bool) 0
myInt= (int) 0
myFloat= (double) 0
myString= (string) 
myChars= (char*) 
myNum= (string) zero
myChoices= (vector<string>) []
myChoice= (string)

myBool = 0
myInt = 0
myFloat = 0.000000
myString = 
myChars = 
myNum = zero


EOF
) && echo ok || echo fail

./test --bool=true | diff -Bby --suppress-common-lines - <(cat - <<EOF
myBool= (bool) 1
myInt= (int) 0
myFloat= (double) 0
myString= (string) 
myChars= (char*) 
myNum= (string) zero
myChoices= (vector<string>) []
myChoice= (string)

myBool = 1
myInt = 0
myFloat = 0.000000
myString = 
myChars = 
myNum = zero


EOF
) && echo ok || echo fail

./test -b -s'Short String' | diff -Bby --suppress-common-lines - <(cat - <<EOF
myBool= (bool) 1
myInt= (int) 0
myFloat= (double) 0
myString= (string) 
myChars= (char*) Short String
myNum= (string) zero
myChoices= (vector<string>) []
myChoice= (string)

myBool = 1
myInt = 0
myFloat = 0.000000
myString = 
myChars = Short String
myNum = zero


EOF
) && echo ok || echo fail

./test '--chars=Short String' | diff -Bby --suppress-common-lines - <(cat - <<EOF
myBool= (bool) 0
myInt= (int) 0
myFloat= (double) 0
myString= (string) 
myChars= (char*) Short String
myNum= (string) zero
myChoices= (vector<string>) []
myChoice= (string)

myBool = 0
myInt = 0
myFloat = 0.000000
myString = 
myChars = Short String
myNum = zero


EOF
) && echo ok || echo fail

./test -s 'Short String' | diff -Bby --suppress-common-lines - <(cat - <<EOF
myBool= (bool) 0
myInt= (int) 0
myFloat= (double) 0
myString= (string) 
myChars= (char*) Short String
myNum= (string) zero
myChoices= (vector<string>) []
myChoice= (string)

myBool = 0
myInt = 0
myFloat = 0.000000
myString = 
myChars = Short String
myNum = zero


EOF
) && echo ok || echo fail

./test --chars 'Short String' | diff -Bby --suppress-common-lines - <(cat - <<EOF
myBool= (bool) 0
myInt= (int) 0
myFloat= (double) 0
myString= (string) 
myChars= (char*) Short String
myNum= (string) zero
myChoices= (vector<string>) []
myChoice= (string)

myBool = 0
myInt = 0
myFloat = 0.000000
myString = 
myChars = Short String
myNum = zero


EOF
) && echo ok || echo fail

./test -bi2 -s string -f 3.4 | diff -Bby --suppress-common-lines - <(cat - <<EOF
myBool= (bool) 1
myInt= (int) 2
myFloat= (double) 3.4
myString= (string) 
myChars= (char*) string
myNum= (string) zero
myChoices= (vector<string>) []
myChoice= (string)

myBool = 1
myInt = 2
myFloat = 3.400000
myString = 
myChars = string
myNum = zero


EOF
) && echo ok || echo fail

./test --string str --bool=false --chars=charstring --float=3.24 --int 5 -i6 | diff -Bby --suppress-common-lines - <(cat - <<EOF
myBool= (bool) 0
myInt= (int) 6
myFloat= (double) 3.24
myString= (string) str
myChars= (char*) charstring
myNum= (string) zero
myChoices= (vector<string>) []
myChoice= (string)

myBool = 0
myInt = 6
myFloat = 3.240000
myString = str
myChars = charstring
myNum = zero


EOF
) && echo ok || echo fail

./test --string str --bool=false --chars=charstring --float=3.24 --int 5 | diff -Bby --suppress-common-lines - <(cat - <<EOF
myBool= (bool) 0
myInt= (int) 5
myFloat= (double) 3.24
myString= (string) str
myChars= (char*) charstring
myNum= (string) zero
myChoices= (vector<string>) []
myChoice= (string)

myBool = 0
myInt = 5
myFloat = 3.240000
myString = str
myChars = charstring
myNum = zero


EOF
) && echo ok || echo fail

./test --string=str --bool=false --chars=charstring -f3.24 | diff -Bby --suppress-common-lines - <(cat - <<EOF
myBool= (bool) 0
myInt= (int) 0
myFloat= (double) 3.24
myString= (string) str
myChars= (char*) charstring
myNum= (string) zero
myChoices= (vector<string>) []
myChoice= (string)

myBool = 0
myInt = 0
myFloat = 3.240000
myString = str
myChars = charstring
myNum = zero


EOF
) && echo ok || echo fail

./test --string=str --bool=false -- --chars=charstring -f3.24 | diff -Bby --suppress-common-lines - <(cat - <<EOF
myBool= (bool) 0
myInt= (int) 0
myFloat= (double) 0
myString= (string) str
myChars= (char*) 
myNum= (string) zero
myChoices= (vector<string>) []
myChoice= (string)
0=--chars=charstring
1=-f3.24

myBool = 0
myInt = 0
myFloat = 0.000000
myString = str
myChars = 
myNum = zero


EOF
) && echo ok || echo fail

./test --string str --bool=false --f --chars=charstring --float=3.24 --int=5 | diff -Bby --suppress-common-lines - <(cat - <<EOF
myBool= (bool) 0
myInt= (int) 5
myFloat= (double) 3.24
myString= (string) str
myChars= (char*) charstring
myNum= (string) zero
myChoices= (vector<string>) []
myChoice= (string)
unknown option 0=--f

myBool = 0
myInt = 5
myFloat = 3.240000
myString = str
myChars = charstring
myNum = zero


EOF
) && echo ok || echo fail

./test --string str --bool=false --chars=charstring --float=3.24 --int=5 | diff -Bby --suppress-common-lines - <(cat - <<EOF
myBool= (bool) 0
myInt= (int) 5
myFloat= (double) 3.24
myString= (string) str
myChars= (char*) charstring
myNum= (string) zero
myChoices= (vector<string>) []
myChoice= (string)

myBool = 0
myInt = 5
myFloat = 3.240000
myString = str
myChars = charstring
myNum = zero

EOF
) && echo ok || echo fail

./test --num two | diff -Bby --suppress-common-lines - <(cat - <<EOF
myBool= (bool) 0
myInt= (int) 0
myFloat= (double) 0
myString= (string)
myChars= (char*)
myNum= (string) two
myChoices= (vector<string>) []
myChoice= (string)

myBool = 0
myInt = 0
myFloat = 0.000000
myString = 
myChars = 
myNum = two

EOF
) && echo OK choices success || echo Fail choices success

./test --num four | diff -Bby --suppress-common-lines - <(cat - <<EOF
Value for option myNum is not one of the valid choices (zero, one, two).
Parse error on --num.
  Argv = ./test --num four
EOF
) && echo OK choices failure || echo Fail choices failure

./test --choices "one two three" --choice one | diff -Bby --suppress-common-lines - <(cat - <<EOF
myBool= (bool) 0
myInt= (int) 0
myFloat= (double) 0
myString= (string)
myChars= (char*)
myNum= (string) zero
myChoices= (vector<string>) [one, two, three]
myChoice= (string) one

myBool = 0
myInt = 0
myFloat = 0.000000
myString = 
myChars = 
myNum = zero

EOF
) && echo OK choices success || echo Fail choices success

./test --choices "one two three" --choice four | diff -Bby --suppress-common-lines - <(cat - <<EOF
Value for option myChoice is not one of the valid choices (one, two, three).
Parse error on --choice.
  Argv = ./test --choices one two three --choice four
EOF
) && echo OK choices fail || echo Fail choices fail
