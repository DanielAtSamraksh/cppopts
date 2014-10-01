#ifndef _ARGV2OBJ_H_
#define _ARGV2OBJ_H_

#include <cstring>  /* strncmp, strcmp, strlen, strchr */
#include <string.h> /* strndup */
#include <stdio.h>  /* printf */
#include <cstdlib>  /* atoi, exit */
#include <string>   /* string */
#include <cctype>   /* tolower */

using std::string;

int min (int i, int j) { return i < j? i: j; };

bool startswith ( const string s1, const char* s2) { 
  return startswith ( s1.c_str(), s2 ); 
};

/* The second argument starts with the first argument. */
bool startswith ( const char* s1, const char* s2 ) {
  int n = strlen( s1 ); 
  return n <= strlen( s2 ) && 0 == strncmp ( s1, s2, n );
};

bool eq ( const string s1, const char* s2) { 
  return eq ( s1.c_str(), s2 ); 
};

bool eq ( const char* s1, const char* s2 ) {
  return strcmp ( s1, s2 ) == 0; 
};

bool atoval ( const char *s, int &i ) {
  printf("converting %s to int %d\n", s, atoi(s));
  i = atoi ( s );
  return true;
};

bool atoval ( const char *s, bool &b ) {
  if ( eq ( s, "1" )) b = true;
  else if ( eq ( s, "0" )) b = false;
  else if ( eq ( s, "true" )) b = true;
  else if ( eq ( s, "false" )) b = false;
  else {
    printf ( "Value error: %s is not a bool.\n", s );
    // exit ( 1 );
    return false;
  }
  printf("converting %s to bool %d\n", s, b);
  return true;
};

bool atoval ( const char *s, string &str ) {
  str = (string) s;
  printf("converting %s to string %s\n", s, str.c_str());
  return true;
}

bool atoval ( const char *s, char *&str ) {
  str = strdup ( s );
  printf("converting %s to string %s\n", s, str);
  return true;
}

bool atoval ( const char *s, double &f ) {
  f = atof(s);
  printf("converting %s to double %f\n", s, f);
  return true;
}

char* tolower(char* s) {
  // from http://stackoverflow.com/a/2661788/268040
  printf("lowering %s\n", s);
  for ( char* p=s; *p; ++p ) *p = tolower(*p);
  return s;
};

class parameters_t {
 public:
  /// Return true if flag matches. Set variable as side effect.
  parameters_t () {};
  parameters_t ( int argc, char ** argv ) { this->init(argc, argv); };
  void init(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
      printf( "\nargv[%d] = %s\n", i, argv[i] );
      
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
	// find the value field, boolean flags are special because
	// they can only be set with explicit equal sign.
	char *val, *boolval;
	char *eqsign = strchr ( argv[i], '=' ); // An equal sign precedes the value
	if ( eqsign ) { *eqsign = '\0'; val = eqsign+1;  boolval = val; }
	else if ( i + 1 < argc )      { val = argv[i+1]; boolval = 0; }
	else                          { val = 0;         boolval = 0; }

	printf( "processing long option %s with possible value %s, bool value %s\n",
		argv[i], val, boolval );
	
	// check the long parameters, atoval will do the right thing
	// because it tracks the type of the variable.
	if ( eq ( "--bool", argv[i] ))   {
	  atoval ( boolval, this->longBool );
	}
	else if ( eq ( "--int", argv[i] ))    {
	  atoval ( val, this->longInt );
	  if ( ! eqsign ) i++; // skip the value parameter if it
			       // wasn't attached, must do this for
			       // all non-bools.
	}
	else if ( eq ( "--float", argv[i] )) {
	  atoval ( val, this->longFloat );
	  if ( ! eqsign ) i++; // skip the value parameter
	}
	else if ( eq ( "--string", argv[i] )) {
	  atoval ( val, this->longString );
	  if ( ! eqsign ) i++; // skip the value parameter
	}
	else if ( eq ( "--chars", argv[i] )) {
	  atoval ( val, this->longChars );
	  if ( ! eqsign ) i++; // skip the value parameter
	}
	else {
	  printf( "Unknown long parameter %s\n", argv[i] );
	}
      } // end long arguments

      else if ( startswith ( "-", argv[i] )) { // process short parameter
	// short arguments can be concatenated, so we just process one char at a time.
	int len = strlen(argv[i]);
	for ( int j = 1; j < len; j++ ) {
	  // the val is the rest of this parameter or the next
	  bool attached = j+1 < len;
	  char *val = attached? argv[i]+j+1: i+1 < argc? argv[i+1]: 0;

	  printf ( " short arg, processing %c, attached %d, val %s\n",
		   argv[i][j], attached, val );
	  
	  if ( argv[i][j] == 'b' ) {
	    this->shortBool = true; continue; // booleans just continue
	  }
	  if ( argv[i][j] == 'S' ) {
	    this->shortString = val; if (!attached) i++; break;
	  }
	  if ( argv[i][j] == 's' ) {
	    this->shortChars = val; if (!attached) i++; break;
	  }
	  if ( argv[i][j] == 'i' ) {
	    atoval ( val, this->shortInt ); if (!attached) i++; break;
	  }
	  if ( argv[i][j] == 'f' ) {
	    atoval ( val, this->shortFloat ); if (!attached) i++; break;
	  }
	  printf( "unknown short parameter %c\n", argv[i][j] );
	}
      } // end short arguments

      else { // not prefixed by -
	printf("skipping parameter %s\n", argv[i]);
      }
    } // end process argv

    // check 
    printf ( "\nparameters are: \n" );
    printf ( " short bool = %d\n", this->shortBool );
    printf ( " short int = %d\n", this->shortInt );
    printf ( " short float = %f\n", this->shortFloat );
    printf ( " short string = %s\n", this->shortString.c_str() );
    printf ( " short chars = %s\n", this->shortChars );

    printf ( " long bool = %d\n", this->longBool );
    printf ( " long int = %d\n", this->longInt );
    printf ( " long float = %f\n", this->longFloat );
    printf ( " long string = %s\n", this->longString.c_str() );
    printf ( " long chars = %s\n", this->longChars );

  };

  bool shortBool, longBool;
  int shortInt, longInt;
  double shortFloat, longFloat;
  string shortString, longString;
  char *shortChars, *longChars;

};

#endif //  _ARGV2OBJ_H_
