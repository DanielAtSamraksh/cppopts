#ifndef _ARGV2OBJ_H_
#define _ARGV2OBJ_H_

#include <cstring>  /* strncmp, strcmp, strlen */
#include <string.h> /* strndup */
#include <stdio.h>  /* printf */
#include <cstdlib>  /* atoi, exit */
#include <string>   /* string */

using std::string;

int min (int i, int j) { return i < j? i: j; }

bool startswith ( const string s1, const char* s2) { 
  return startswith ( s1.c_str(), s2 ); 
}

/* The second argument starts with the first argument. */
bool startswith ( const char* s1, const char* s2 ) {
  int n = strlen( s1 ); 
  return n <= strlen( s2 ) && 0 == strncmp ( s1, s2, n );
}

bool eq ( const string s1, const char* s2) { 
  return eq ( s1.c_str(), s2 ); 
}

bool eq ( const char* s1, const char* s2 ) {
  return strcmp ( s1, s2 ) == 0; 
}

bool convert ( char *s, int &i ) { i = atoi ( s ); return true; }

bool convert ( char *s, bool &b ) {
  if ( eq ( s, "1" )) b = true;
  else if ( eq ( s, "0" )) b = false;
  else if ( eq ( s, "true" )) b = true;
  else if ( eq ( s, "false" )) b = false;
  else {
    printf ( "Value error: %s is not a bool.\n", s );
    // exit ( 1 );
    return false;
  }
  return true;
}

class parameters_t {
 public:
  void process_long_parameter( int argc, char **argv, int &i){
    char *a = argv[i];
    printf("process_long_parameter %s\n", a);

    if ( eq ("--long-bool", a )) { this->longBool = true; }
    else if ( startswith ( "--long-bool=", a )) {
      convert(a+strlen("--long-bool="), this->longBool );
    }

    else {
      printf("Unrecognized long parameter %s\n", argv[i]);
      // exit ( 1 );
    }
  };

  void process_short_parameter( int argc, char **argv, int &i ) {
    printf("process_short_parameter %s\n", argv[i]);
    for ( char* a = argv[i]+1; *a != '\0'; a++ ) {
      if ( eq ( "i", a )) { this->i = true; }

      else if ( eq ( "n", a )) {
	if ( i+1 >= argc ) {
	  printf("Missing argument for %s\n", a);
	  exit ( 1 );
	}
	i++;
	convert ( argv[i], this->n );
	break;
      }
      else if ( startswith ( "n", a )) { 
	convert ( a+1, this->n );
	break;
      }

      else {
	printf("Unrecognized short parameter %s\n", a);
	// exit ( 1 );
	break;
      }
    }
  };
  
  parameters_t () {};
  parameters_t ( int argc, char ** argv ) { this->init(argc, argv); };
  void init(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
      // -- no more processing
      if ( eq ( "--", argv[i] )) { 
	printf ( "End of arguments reached. The rest of the arguments are:\n" );
	for (; i < argc; i++) {
	  printf("  %s\n", argv[i]);
	}
      }
      else if ( eq ( "-", argv[i] )) {
	printf ( "Single dash found\n" );
      }
      else if ( startswith ( "--", argv[i] )) { // process long parameter
	process_long_parameter(argc, argv, i);
      }
      else if ( startswith ( "-", argv[i] )) { // process short parameter
	process_short_parameter(argc, argv, i);
      }
      else {
	printf("skipping parameter %s\n", argv[i]);
      }
    }

    // check
    printf ("parameters are: \n int n = %d\n bool i = %d\n bool longBool = %d\n\n",
	    this->n, this->i, this->longBool );

    
  };
  int n; 
  bool i, longBool;

};

#endif //  _ARGV2OBJ_H_
