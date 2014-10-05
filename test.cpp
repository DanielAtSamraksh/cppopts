#include "cppopts.h"

struct testParameters_t : parameters_t {
  bool myBool;
  int myInt;
  double myFloat;
  string myString;
  char *myChars;
  
  testParameters_t() {
    options.push_back (
      new parameter_t < bool > (
	&(this->myBool), "myBool", 'b', (char*) "bool", false, "help bool" ));
    options.push_back (
      new parameter_t < int > (
	&(this->myInt), "myInt", 'i', (char*) "int", 0, "help int" ));
    options.push_back (
      new parameter_t < double > (
	&(this->myFloat), "myFloat", 'f', (char*) "float", 0.0, "help float" ));
    options.push_back (
      new parameter_t < string > (
	&(this->myString), "myString", 'S', (char*) "string", "", "help for myString." ));
    options.push_back (
      new parameter_t < char* > (
	&(this->myChars), "myChars", 's', (char*) "chars", (char *) "", "help for myChars" ));
  };
};

int main ( int argc, const char** argv ) {
  testParameters_t args;
  args.parse( argc, argv );
  printf("%s\n", args.dump().c_str());

  printf("myBool = %d\n", args.myBool);
  printf("myInt = %d\n", args.myInt);
  printf("myFloat = %f\n", args.myFloat );
  printf("myString = %s\n", args.myString.c_str() );
  printf("myChars = %s\n", args.myChars );
  
  printf ( "\n\n" );

}
