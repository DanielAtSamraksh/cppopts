#include "commandline.h"

using std::stringstream;
using std::cout;

int main ( int argc, char** argv ) {

  int x=1, y=2;
  float pi=3.14;
  string str;

  opts_t opts; 
  opts
    .addValue("x", "x value", &x)
    .addValue("y", "y value", &y)
    .addChoice( 1 )
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

};
