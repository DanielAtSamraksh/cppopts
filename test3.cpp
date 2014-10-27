#include "commandline.h"

using std::stringstream;
using std::cout;

int main ( int argc, char** argv ) {

  int x=1, y=2;  // declare two ints with default values
  float pi=3.14; // declare float with default value
  string str="default";    // declare string with default value

  opts_t opts; // declare an opts_t instance and configure it.
  opts
    .addValue("x", "x value", &x) // add option for int x
    .addValue("y", "y value", &y) // add option for int y
    .addChoice( 1 )               // add choices for y (the last option)
    .addChoice( 2 )
    .addChoice( 3 )
    .addValue("pi", "pi value", &pi)
    .addValue("string", "string value", &str);

  cout << "Usage:\n" << argv[0] << " [options]\n" 
       << opts.usage() << "\n";

  if ( ! opts.parse ( argc-1, argv+1 ) ) {
    cout << "error parsing\n";
    cout << opts.usage();
    exit (1);
  }
  
  cout << "Dumping values:\n" << opts.dump();

};
