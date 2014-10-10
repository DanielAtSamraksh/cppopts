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
      &((*(new parameter_t < bool >))
	.addName("myBool").addPtr(&(this->myBool))
	.addShort('b').addLong("bool").addDefaultValue(false)
	.addHelp("help bool")));


    // myInt
    options.push_back (
      &((*(new parameter_t < int >))
	.addPtr(&(this->myInt)).addName("myInt")
	.addShort('i').addLong("int").addDefaultValue(0)
	.addHelp("help int")));

    // myFloat
    options.push_back (
      &((*(new parameter_t < double >))
	.addPtr( &(this->myFloat) )
	.addName( "myFloat" )
	.addShort( 'f' )
	.addLong( "float" )
	.addDefaultValue ( 0.0 )
	.addHelp ( "help float" )));

    // myString
    options.push_back (
      &((*(new parameter_t < string >))
	.addPtr( &(this->myString) )
	.addName( "myString" )
	.addShort( 'S' )
	.addLong( "string" )
	.addHelp( "help for myString." )));

    // myChars
    options.push_back (
      &((*(new parameter_t < char* >))
	.addPtr( &(this->myChars) )
	.addName( "myChars" )
	.addShort( 's' )
	.addLong( "chars" )
	.addDefaultValue ( (char*) "" )
	.addHelp ( "help for myChars" )));

    // myNum - user must select between options stored in vector
    // stringv note the "new". Without the new, stringv goes out of
    // scope when the constructor finishes. We want it to persist.
    options.push_back (
      &((*(new parameter_t < string >))
	.addPtr( &(this->myNum) )
	.addName( "myNum" )
	.addShort( 'n' )
	.addLong( "num" )
	.addHelp( "one of zero, one, or two" )
	.addDefaultValue( "zero" )
	.addChoice( "zero" )
	.addChoice( "one" )
	.addChoice( "two" )));

    // Dynamic choice, pass in the choices in one option (myChoices)
    // and use them to verify another option (myChoice).

    // myChoices

      
    parameter_t < vector<string> > *myChoices_ = new parameter_t< vector <string> >;
    (*myChoices_)
      .addPtr( &(this->myChoices) )
      .addName( "myChoices" )
      .addLong( "choices" )
      .addHelp( "Possible choices." );
    options.push_back ( myChoices_ );
    
    // myChoice - use value of myChoices to screen this input.
    options.push_back (
      &((*(new parameter_t < string >))
	.addPtr( &(this->myChoice) )
	.addName( "myChoice" )
	.addLong( "choice" )
	.addHelp( "My choice." )
	.addChoices( &(myChoices_->value) )));

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
