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
) && echo ok short flag || echo Fail short flag

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
) && echo OK long flag false || echo Fail long flag false

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
) && echo OK long flag true || echo Fail long flag true

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
) && echo OK short string || echo Fail short string 

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
) && echo OK long string || echo Fail long string

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
) && echo OK short string separated || echo Fail short string separated

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
) && echo OK long string separated || echo Fail long string separated

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
) && echo OK float separated || echo Fail float separated

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
) && echo OK long float || echo Fail long float

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
) && echo OK long float || echo Fail long float

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
) && echo OK long chars || echo Fail long chars

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
) && echo OK double dash || echo Fail double dash

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
) && echo OK unknown long || echo Fail unknown long 

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
) && echo OK long int || echo Fail long int

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
) && echo OK right choice || echo Fail right choice

./test --num four | diff -Bby --suppress-common-lines - <(cat - <<EOF
Value for option myNum is not one of the valid choices (zero, one, two).
Parse error on --num.
  Argv = ./test --num four
EOF
) && echo OK wrong choice || echo Fail wrong choice

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
) && echo OK right dynamic choice || echo Fail right dynamic choice

./test --choices "one two three" --choice four | diff -Bby --suppress-common-lines - <(cat - <<EOF
Value for option myChoice is not one of the valid choices (one, two, three).
Parse error on --choice.
  Argv = ./test --choices one two three --choice four
EOF
) && echo OK wrong dynamic choice || echo Fail wrong dynamic choice
