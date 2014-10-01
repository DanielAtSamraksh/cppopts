#ifndef _ARGV2OBJ_H_
#define _ARGV2OBJ_H_

#include <cstring>  /* strncmp, strcmp, strlen, strchr */
#include <string.h> /* strndup */
#include <stdio.h>  /* printf */
#include <cstdlib>  /* atoi, exit */
#include <string>   /* string */
#include <cctype>   /* tolower */
#include <typeinfo> /* (typeid result).name == "bool" */
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
  // printf("converting %s to int %d\n", s, atoi(s));
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
  // printf("converting %s to bool %d\n", s, b);
  return true;
};

bool atoval ( const char *s, string &str ) {
  str = (string) s;
  // printf("converting %s to string %s\n", s, str.c_str());
  return true;
}

bool atoval ( const char *s, char *&str ) {
  str = strdup ( s );
  // printf("converting %s to string %s\n", s, str);
  return true;
}

bool atoval ( const char *s, double &f ) {
  f = atof(s);
  // printf("converting %s to double %f\n", s, f);
  return true;
}

char* tolower(char* s) {
  // from http://stackoverflow.com/a/2661788/268040
  printf("lowering %s\n", s);
  for ( char* p=s; *p; ++p ) *p = tolower(*p);
  return s;
};


class flag_t {
 public:
  bool value;
  char *help;     // help text
  char *shorts;   // short arguments
  char **longv;   // long arguments
  int longc;  // number of long arguments

  flag_t ( ) {};
  flag_t ( char *_shorts, char** _longv, int _longc ) {
    init(_shorts, _longv, _longc);
  };
  
  void init ( char *_shorts, char** _longv, int _longc ) {
    this->shorts = _shorts;
    this->longv = _longv;
    this->longc = _longc;
  };
  
  /// Parse argv starting at argv[i][j]. Return true on error. Update
  /// i and j as needed.
  bool parse ( int &i, int &j, const int argc, const char **argv ) {
    // i = index into argv, j = index into argv[i]
    if ( i >= argc) return false; // no more arguments
    if ( j > 0 && j >= strlen ( argv[i] )) { 
      i++; j = 0; // move to next argument if at the end argv[i]
      if ( i >= argc) return false; // return if no more arguments
    }
    if ( '-' == argv[i][0] ) {
      if ('-' == argv[i][1]) {
	// -- means stop processing
	if ('\0' == argv[i][2]) return false;
	j=2;
	return this->parselong ( i, j, argc, argv );
      }
      if (j < 1) j = 1;
      printf("parsing short i=%d j=%d %c\n", i, j, argv[i][j]);
      return parseshort ( i, j, argc, argv );
    }
    return false; // not long or short option, just return
  };

  bool parselong ( int &i, int &j, const int argc, const char **argv ) {
    // we start pointing at the long
    // printf("checking %s, parsing long,  %s %s\n", this->longv[0], argv[i]+j, argv[i]);
    // printf("i = %d, j = %d\n", i, j);
    
    for ( int _i = 0; _i < this->longc; _i++ ) {
      // printf("checking %s, parsing long,  %s %s\n", this->longv[_i], argv[i]+j, argv[i]);
      char *l = this->longv[_i];
      if ( startswith ( l, argv[i]+2 ) ) {
	// printf("startswith %s %s\n", this->longv[_i], argv[i]+2);
	char char_after = argv[i][ 2 + strlen(l) ];
	switch ( char_after ) {
	case '\0': // value in next argument
	  this->value = true;
	  i++; j = 0;
	  return false;
	case '=': // value in this argument
	  atoval ( argv[i]+2+strlen(l)+1, this->value );
	  i += 1; j = 0;
	  return false;
	default: ; // is longer than what we're looking for
	}
      }
    }
    return false;
  };
  
  bool parseshort ( int &i, int &j, const int argc, const char **argv ) {
    // check shorts
    printf("flag parseshort\n");
    if (j < 1) { printf("parse error, j <= 0\n"); exit(1); }
    for ( int _i = strlen(this->shorts)-1; _i >= 0; _i-- ) {
      if ( this->shorts[_i] == argv[i][j] ) {
	this->value = true;
	j++;
	return false;
      }
    }
    return false;
  };

};

template < class T>
class parameter_t {
 public:
  T value;
  char *help;     // help text
  char *shorts;   // short arguments
  char **longv;   // long arguments
  int longc;  // number of long arguments

  void init ( char *_shorts, char** _longv, int _longc ) {
    this->shorts = _shorts;
    this->longv = _longv;
    this->longc = _longc;
  };
  
  /// Parse argv starting at argv[i][j]. Return true on error. Update
  /// i and j as needed.
  bool parse ( int &i, int &j, const int argc, const char **argv ) {
    // i = index into argv, j = index into argv[i]
    if ( i >= argc) return false; // no more arguments
    if ( j > 0 && j >= strlen ( argv[i] )) { 
      i++; j = 0; // move to next argument if at the end argv[i]
      if ( i >= argc) return false; // return if no more arguments
    }
    if ( '-' == argv[i][0] ) {
      if ('-' == argv[i][1]) {
	// -- means stop processing
	if ('\0' == argv[i][2]) return false;
	j=2;
	return this->parselong ( i, j, argc, argv );
      }
      if (j < 1) j = 1;
      // printf("parsing short i=%d j=%d %c\n", i, j, argv[i][j]);
      return parseshort ( i, j, argc, argv );
    }
    return false; // not long or short option, just return
  };

  bool parselong ( int &i, int &j, const int argc, const char **argv ) {
    // we start pointing at the long
    // printf("checking %s, parsing long,  %s %s\n", this->longv[0], argv[i]+j, argv[i]);
    // printf("i = %d, j = %d\n", i, j);
    
    for ( int _i = 0; _i < this->longc; _i++ ) {
      // printf("checking %s, parsing long,  %s %s\n", this->longv[_i], argv[i]+j, argv[i]);
      char *l = this->longv[_i];
      if ( startswith ( l, argv[i]+2 ) ) {
	// printf("startswith %s %s\n", this->longv[_i], argv[i]+2);
	char char_after = argv[i][ 2 + strlen(l) ];
	switch ( char_after ) {
	case '\0': // value in next argument
	  // printf("type = %s\n", typeid(T).name());
	  if (! strncmp(typeid(T).name(), "bool", 1)) {
	    atoval("true", this->value);
	    i++; j = 0;
	  }
	  else if ( i+1 >= argc ) {
	    printf("Missing argument for option %s\n", argv[i]);
	    i++; j=0;// avoid infinite loop
	    return true;
	  }
	  else {
	    atoval (argv[i+1], this->value);
	    i += 2; j = 0;
	  }
	  return false;
	case '=': // value in this argument
	  atoval ( argv[i]+2+strlen(l)+1, this->value );
	  i += 1; j = 0;
	  return false;
	default: ; // is longer than what we're looking for
	}
      }
    }
    return false;
  };
  
  bool parseshort ( int &i, int &j, const int argc, const char **argv ) {
    // check shorts
    if (j < 1) { printf("parse error, j <= 0\n"); exit(1); }
    for ( int _i = strlen(this->shorts)-1; _i >= 0; _i-- ) {
      // printf("type = %s\n", typeid(T).name());
      if ( this->shorts[_i] == argv[i][j] ) {
	// printf("%c matches %c, in %s, 2 over: %s\n", argv[i][j], this->shorts[_i], argv[i], argv[i]+2);
	if (! strncmp(typeid(T).name(), "bool", 1)) {
	  atoval("true", this->value);
	  j++;
	}
	else if (j+1 < strlen(argv[i])) {
	  atoval ( argv[i]+j+1, this->value );
	  i++; j = 0;
	}
	else if ( i + 1 < argc ) {
	  atoval ( argv[i+1], this->value );
	  i += 2; j=0;
	}
	else {
	  printf ("missing value for short option %c in %s\n", this->shorts[_i], argv[i]);
	  i++; j=0; // avoid infinite loop if return value is not checked.
	  return true;
	}
	return false;
      }
    }
    return false;
  };

  parameter_t () {};
  parameter_t ( char *_shorts, char** _longv, int _longc ) {
    init(_shorts, _longv, _longc);
  };
  
};


class parameters_t {
 public:

  //  flag_t myBool;
  parameter_t < bool > myBool;
  parameter_t < int > myInt;
  parameter_t < double > myFloat;
  parameter_t < string > myString;
  parameter_t < char* > myChars;
  
  
  /// Return true if flag matches. Set variable as side effect.
  parameters_t () {};
  parameters_t ( int argc, const char ** argv ) { this->init(argc, argv); };
  void init(int argc, const char **argv) {
    
    char *cs[] = {(char*) "bool", (char*) "my-bool"};
    this->myBool.init((char*) "b", cs, 2);
    
    char *cs2[] = {(char*) "int", (char*) "my-int"};
    this->myInt.init((char*) "i", cs2, 2);

    char *cs3[] = {(char*) "float", (char*) "my-float"};
    this->myFloat.init((char*) "f", cs3, 2);
    
    char *cs4[] = {(char*) "string", (char*) "my-string"};
    this->myString.init((char*) "S", cs4, 2);
    
    char *cs5[] = {(char*) "chars", (char*) "my-chars"};
    this->myChars.init((char*) "s", cs5, 2);
  
    int i=1, j=0;
    int oldi, oldj;
    do {
      oldi = i; oldj = j;
      myBool.parse ( i, j, argc, argv );
      myInt.parse ( i, j, argc, argv );
      myFloat.parse ( i, j, argc, argv );
      myString.parse ( i, j, argc, argv );
      myChars.parse ( i, j, argc, argv );
    } while ( oldi != i || oldj != j );

    // check 
    printf ( "\nparameters are: \n" );
    printf ( " bool = %d\n", this->myBool.value );
    printf ( " int = %d\n", this->myInt.value );
    printf ( " float = %f\n", this->myFloat.value );
    printf ( " string = %s\n", this->myString.value.c_str() );
    printf ( " chars = %s\n", this->myChars.value );
    printf ( "\n\n" );
  };

};

#endif //  _ARGV2OBJ_H_
