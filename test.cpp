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

  vector <string> myChoices;
  string myChoice;
  
  /// Create a constuctor which declares the options we're interested in.
  testParameters_t() {

    // myBool
    options.push_back (
      new parameter_t < bool > (
	&(this->myBool), "myBool", 'b', (char*) "bool", false, "help bool" ));

    // myInt
    options.push_back (
      new parameter_t < int > (
	&(this->myInt), "myInt", 'i', (char*) "int", 0, "help int" ));

    // myFloat
    options.push_back (
      new parameter_t < double > (
	&(this->myFloat), "myFloat", 'f', (char*) "float", 0.0, "help float" ));

    // myString
    options.push_back (
      new parameter_t < string > (
	&(this->myString), "myString", 'S', (char*) "string", "", "help for myString." ));

    // myChars
    options.push_back (
      new parameter_t < char* > (
	&(this->myChars), "myChars", 's', (char*) "chars", (char *) "", "help for myChars" ));

    // myNum - user must select between options stored in vector
    // stringv note the "new". Without the new, stringv goes out of
    // scope when the constructor finishes. We want it to persist.
    vector <string> *stringv = new vector<string>; 
    stringv->push_back("zero");
    stringv->push_back("one");
    stringv->push_back("two");
    parameter_t < string > *stringp = new parameter_t < string > (
      &(this->myNum), "myNum", 'n', (char*) "num",
      "zero", "one of zero, one, or two");
    options.push_back ( stringp );
    stringp->choices = stringv; // set the choices member to the choices.


    // Dynamic choice, pass in the choices in one option (myChoices)
    // and use them to verify another option (myChoice).

    // myChoices
    parameter_t < vector<string> > * choicesp =
      new parameter_t < vector<string> > (
	&(this->myChoices), "myChoices", (char*) "choices",
	vector<string>(), "Possible choices." );
    options.push_back ( choicesp );
    
    // myChoice - use value of myChoices to screen this input.
    parameter_t < string > *choicep = new parameter_t < string > (
      &(this->myChoice), "myChoice", (char*) "choice", "", "My choice.");
    options.push_back ( choicep );
    choicep->choices = &(choicesp->value);

  };
};

int main ( int argc, const char** argv ) {
  testParameters_t args;
  if (! args.parse( (unsigned) argc, argv )) { return 0; }
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
