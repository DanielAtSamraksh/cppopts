#include "cppopts2.h"

using std::stringstream;
using std::cout;

int main ( int argc, char** argv ) {

  int x=1, y=2;
  float pi=3.14;

  opts_t opts; 
  opts
    .addValue("x", "x value", &x)
    .addValue("y", "y value", &y)
    .addValue("pi", "pi value", &pi);

  if ( ! opts.parse ( argc-1, argv+1 ) ) {
    cout << "error parsing\n";
    cout << opts.usage();
    exit (1);
  }

};
