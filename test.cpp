#include "cppopts.h"

/// Subclass parameter_t to reflect *our* particular parameters.
struct testParameters_t : parameters_t {
  // These are our parameters listed out, so that they can be accessed directly.
  // Parameters can also be accessed 
  bool myBool;
  int myInt;
  double myFloat;
  string myString;
  char *myChars;
  string myNum;

  /// Create a constuctor which declares the options we're interested in.
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
    vector <string> stringv;
    stringv.push_back("zero");
    stringv.push_back("one");
    stringv.push_back("two");
    options.push_back (
      new parameter_t < string > (
	&(this->myNum), "myNum", 'n', (char*) "num",
	"zero", "one of zero, one, or two", stringv ));
  };
};

int main ( int argc, const char** argv ) {
  testParameters_t args;
  if (! args.parse( argc, argv )) { return 0; }
  printf("%s\n", args.dump().c_str());

  // access the parameters directly.
  printf("myBool = %d\n", args.myBool);
  printf("myInt = %d\n", args.myInt);
  printf("myFloat = %f\n", args.myFloat );
  printf("myString = %s\n", args.myString.c_str() );
  printf("myChars = %s\n", args.myChars );
  printf("myNum = %s\n", args.myNum.c_str() );

  // parameters can also be accessed like this:
  //     dynamic_cast(parameter_t<bool>*)args["myBool"]
  // The dynamic cast is necessary because to args["myBool"] returns an abstractParameter_t*
  // which does not have a value. 
  printf ( "\n\n" );

}
