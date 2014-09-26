#ifndef _ARGV2OBJ_H_
#define _ARGV2OBJ_H_

#include <cstring> /* strncmp, strlen */
#include <stdio.h> /* printf */
#include <cstdlib> /* atoi, exit */
#include <string>  /* string */

using std::string;

int convert ( char * s ) { return atoi ( s ); }

int min (int i, int j) { return i < j? i: j; }

bool eq ( const string s1, const char* s2) { 
  return eq ( s1.c_str(), s2 ); 
}

bool eq ( const char* s1, const char* s2 ) {
  int n = min ( strlen( s1 ), strlen( s2 ) );
  return 0 == strncmp ( s1, s2, n );
}

class parameters_t {
 public:
  parameters_t () {};
  parameters_t ( int argc, char ** argv ) { this->init(argc, argv); };
  void init(int argc, char **argv) {
    char *program = argv[0]; argv++; argc--;
    printf ( "argc = %d\n", argc );

    char *a0 = argv[0]; argc--;
    while ( argc >= 0 ) {
      printf ( "processing %s, argc = %d\n", a0, argc );

      if ( eq ( "-n", a0 )) {
	if ( argc < 0) {
	  printf ( "No value for n. Argc = %d\n", argc ); 
	  exit(1); 
	}
	this-> n = convert ( argv[0] );
	argv++; argc--;
      }

      else if ( eq ( "-i", a0 )) { 
	this -> i = true; 
      }

      a0 = argv[0]; argv++; argc--;
    }
    printf ( "-n = %d\n-i = %d\n\n", this->n, this->i );
  }
  int n; 
  bool i;

};

#endif //  _ARGV2OBJ_H_
