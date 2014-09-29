#ifndef _ARGV2OBJ_H_
#define _ARGV2OBJ_H_

#include <cstring>  /* strncmp, strcmp, strlen */
#include <string.h> /* strndup */
#include <stdio.h>  /* printf */
#include <cstdlib>  /* atoi, exit */
#include <string>   /* string */

using std::string;

int convert ( char * s ) { return atoi ( s ); }

int min (int i, int j) { return i < j? i: j; }

bool startswith ( const string s1, const char* s2) { 
  return startswith ( s1.c_str(), s2 ); 
}

/* The second argument starts with the first argument. */
bool startswith ( const char* s1, const char* s2 ) {
  int n = strlen( s1 ); 
  return n <= strlen( s2 ) || 0 == strncmp ( s1, s2, n );
}

bool eq ( const string s1, const char* s2) { 
  return eq ( s1.c_str(), s2 ); 
}

bool eq ( const char* s1, const char* s2 ) {
  return strcmp ( s1, s2 ) == 0; 
}

class parameters_t {
 public:
  parameters_t () {};
  parameters_t ( int argc, char ** argv ) { this->init(argc, argv); };
  void init(int argc, char **argv) {
    char *program = argv[0];
    //    printf ( "argc = %d\n", argc );
    int attached = -1; // track which arguments are attached.
    int mallocced = -1; // track which arguments are mallocced
    for (int i = 1; i < argc; i++) {
      printf ( "processing argv[%d] = %s\n", i, argv[i] );
      // process parameters in normal form (space separated)

      // short (-n) val
      if ( eq ( ( attached == i? "n": "-n" ), argv[i] )) {
	if ( i >= argc) {
	  printf ( "No value for n, argc = %d\n", argc ); 
	  exit(1); 
	}
	this-> n = convert ( argv[i+1] );
	i++; // skip the next field because it was used as a value
      }

      // -i (bool)
      else if ( eq ( attached == i? "i": "-i", argv[i] )) {
	this -> i = true;
      }

      else if ( eq ( "--long-bool", argv[i] )) {
	if ( attached == i+1 ) {
	  if ( eq ( "true", argv[i+1] ) || eq ( "1", argv[i+1] )) {
	    this -> longBool = true;
	  }
	  else if ( eq ( "False", argv[i+1] ) || eq ( "0", argv[i+1] )) {
	    this -> longBool = false;
	  }
	  else {
	    printf("Can't set %s to unrecognized value %s.\n", argv[i], argv[i+1]);
	    exit(1);
	  }
	  i++;
	}
	else {
	  this -> longBool = false;
	}
      }

      // special cases

      // -- no more processing
      else if ( eq ( "--", argv[i] )) { 
	printf ("End of arguments reached. The rest of the arguments are:\n");
	for (; i < argc; i++) {
	  printf("  %s\n", argv[i]);
	}
      }

      // unrecognized long parameter
      else if ( startswith( "--", argv[i] )) {
	if ( char* sep = strchr( argv[i], '=' )) {
	  // --*=* long parameter set with =
	  // separate
	  *sep = '\0';
	  argv[i-1] = argv[i]; // point the previous argument to this one.
	  argv[i] = sep+1; // point this argument to what's after the equal sign
	  i -= 2; // back up two  to process the previous argument next.
	}
	else { // unknown argument
	  printf ( "Long parameter %s is unknown.\n", argv[i] );
	}
      }

      // unrecognized short parameter
      else if ( attached == i || startswith( "-", argv[i] )) {
	int len = attached == i ? 1: 2;
	if ( strlen( argv[i] ) > len ) { // multiple short args, separate
	  if ( ! ( argv[i-1] = attached == i? strdup("--"): strndup( argv[i], 2 ))) {
	    printf("Can't duplicate string in file %s line %d\n", __FILE__, __LINE__);
	    exit(1);
	  }
	  mallocced = i-1;
	  if ( attached == i ) argv[i-1][1] = argv[i][0]; // copy argument
	  attached = i; // this field is now attached
	  argv[i] += len; // point to the next parameter
	  i -= 2; // back up two  to process the previous argument next.
	}
	else { // unknown short parameter
	  printf ( "Short parameter %s is unknown.\n", argv[i] );
	}
      }
      if (mallocced == i) { free ( argv[i] ); }
    }
    printf ("parameters are: \n int n = %d\n bool i = %d\n bool longBool = %d\n\n",
	    this->n, this->i, this->longBool );
  };
  int n; 
  bool i, longBool;

};

#endif //  _ARGV2OBJ_H_
