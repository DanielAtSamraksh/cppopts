#include "cppopts.h"

int main ( int argc, const char** argv ) {
  testParameters_t args;
  args.parse( argc, argv );
  printf("myBool = %d\n", args.myBool);
  printf("myInt = %d\n", args.myInt);
  printf("myFloat = %f\n", args.myFloat );
  printf("myString = %s\n", args.myString.c_str() );
  printf("myChars = %s\n", args.myChars );
  
  printf ( "_____________________________________________________\n\n" );

}
