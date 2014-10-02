#ifndef _ARGV2OBJ_H_
#define _ARGV2OBJ_H_

#include <cstring>  /* strncmp, strcmp, strlen, strchr */
#include <string.h> /* strndup */
#include <stdio.h>  /* printf */
#include <cstdlib>  /* atoi, exit */
#include <string>   /* string */
#include <sstream>   /* stringstream */
#include <cctype>   /* tolower */
#include <typeinfo> /* (typeid result).name == "bool" */

#include <vector>
#include <map>

using std::string;
using std::vector;
using std::map;
using std::stringstream;

template < class T >
string toString( T x ) {
  stringstream s; s << x; return s.str();
};

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


/// Abstract class used as a parent class for the real template
/// class. This allows storage of an array, vector, or map of pointers
/// to the template class. To work successfully, every method called
/// on the object pointers stored in the array needs to have a virtual
/// method defined here.
struct abstractParameter_t {
  string name;
  char s;   // short argument
  char *l;   // long argument
  string help;     // help text

  //  virtual ~abstractParameter_t() = 0;
  virtual bool parse ( int &i, int &j, const int argc, const char **argv ) = 0;
  virtual const char* c_str() = 0;
  virtual const char* usage() = 0;
  ~abstractParameter_t(){};
};

template < class T >
struct parameter_t: public abstractParameter_t {
 public:

  parameter_t ( T * ptr, string name, char _short, char* _long, T def, string help ) {
    init ( ptr, name, _short, _long, def, help );
  };
  
  // virtual ~parameter_t(){};
  ~parameter_t(){};

  T defaultValue; 
  T value;
  T *valuePtr;

  string _str;

  string str() { 
    stringstream s; s << value; 
    this->_str = s.str(); // save the returned string 
    return this->_str; // return the saved copy
  };

  const char *c_str() {
    return this->str().c_str();
  };

  string _usage;
  const char* usage() {
    stringstream s;
    s << "name: " << name << ", short: -" << this->s << ", long: --" << l << ", defaultValue: " << defaultValue << ", value: " << value << ", help: " << help << "\n";
    _usage = s.str();
    return _usage.c_str();
  };

  void init ( T* ptr, string name, char _short, char* _long, T def, string help ) {
    this->valuePtr = ptr;
    this->s = _short;
    this->l = _long;
    this->value = def;
    if (this->valuePtr) *(this->valuePtr) = def;
    this->defaultValue = def;
    this->help = help;
    this->name = name;
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
    
    if ( startswith ( this->l, argv[i]+2 ) ) {
      char char_after = argv[i][ 2 + strlen(l) ];
      switch ( char_after ) {
      case '\0': // value in next argument
	// printf("type = %s\n", typeid(T).name());
	if (! strncmp(typeid(T).name(), "bool", 1)) {
	  atoval("true", this->value);
	  *(this->valuePtr) = this->value;
	  i++; j = 0;
	}
	else if ( i+1 >= argc ) {
	  printf("Missing argument for option %s\n", argv[i]);
	  i++; j=0;// avoid infinite loop
	  return true;
	}
	else {
	  atoval (argv[i+1], this->value);
	  *(this->valuePtr) = this->value;
	  i += 2; j = 0;
	}
	return false;
      case '=': // value in this argument
	atoval ( argv[i]+2+strlen(l)+1, this->value );
	*(this->valuePtr) = this->value;
	i += 1; j = 0;
	return false;
      default: ; // is longer than what we're looking for
      }
    }
    return false;
  };
  
  bool parseshort ( int &i, int &j, const int argc, const char **argv ) {
    // check shorts
    if (j < 1) { printf("parse error, j <= 0\n"); exit(1); }
    // printf("type = %s\n", typeid(T).name());
    if ( this->s == argv[i][j] ) {
      if (! strncmp(typeid(T).name(), "bool", 1)) {
	atoval("true", this->value);
	*(this->valuePtr) = this->value;
	j++;
      }
      else if (j+1 < strlen(argv[i])) {
	atoval ( argv[i]+j+1, this->value );
	*(this->valuePtr) = this->value;
	i++; j = 0;
      }
      else if ( i + 1 < argc ) {
	atoval ( argv[i+1], this->value );
	*(this->valuePtr) = this->value;
	i += 2; j=0;
      }
      else {
	printf ("missing value for short option %c in %s\n", this->s, argv[i]);
	i++; j=0; // avoid infinite loop if return value is not checked.
	return true;
      }
      return false;
    }
    return false;
  };

};


class parameters_t {
 public:

  parameters_t () {};

  // Use a vector to store options to keep them in order.  Name lookup
  // is linear search which is ok if number of parameters are small.
  vector < abstractParameter_t* > options;
  vector < string > positionals, unknowns; // the part of the argvs not parsed as options

  int size() { return positionals.size(); };

  string operator[] ( int i ) { return positionals[i]; };

  abstractParameter_t* operator[] ( string n ) {
    for ( int i = 0; i < options.size(); i++) {
      if ( n == options[i]->name ) return options[i];
    }
      printf( "Lookup error: Parameter %s not found.\n", n.c_str() );
      exit(1);
  };

  string _usage;
  string usage() {
    stringstream s; 
    for ( int i = 0; i < options.size(); i++ ) {
      s << options[i]->usage();
      printf("%s", options[i]->usage());
    };
    _usage = s.str();
    return _usage;
  };

  
  void parse (int argc, const char **argv) {
    int i=1, j=0;
    int oldi, oldj;
    do { do {
      oldi = i; oldj = j;
      for ( int k = 0; k < options.size(); k++ ) {
	// printf( "  at %s looking for %s\n", argv[i], options[k]->name.c_str() );
	options[k]->parse ( i, j, argc, argv );
      }} while ( oldi != i || oldj != j );
      // we're stuck, find out why
      if ( i >= argc ) break; // done with args
      if ( eq ( "--", argv[i] )) { // end of options, parse positionals
	for ( i++; i < argc; i++ ) positionals.push_back(argv[i]);
	break;
      }
      if ( startswith ( "--", argv[i] ) && j == 2 ) { // unknown long option
	unknowns.push_back(argv[i]);
	i++; j = 0;
	continue;
      }
      if ( startswith ( "-", argv[i] ) && j > 1 ) { // unknown short option
	unknowns.push_back( "-"+ (string)(argv[i]+j) );
	i++; j = 0;
	continue;
      }
      if ( j != 0 ) { // sanity check
	printf("error parsing at %s (argument %d), expected the beginning of a positional\n", argv[i], i);
	exit(1);
      }
      // we've reached a positional argument. Consume it and continue.
      positionals.push_back ( argv[i] ); i++; continue;
    } while (1); // 

    // check
    printf("\nAfter parsing\n");
    if (unknowns.size()) {
      printf ( "unknowns: ");
      for (int i = 0; i < unknowns.size(); i++) {
	printf("%s ", unknowns[i].c_str());
      }
      printf("\n\n");
    }
    if (positionals.size()) {
      printf ( "positionals: ");
      for (int i = 0; i < positionals.size(); i++) {
	printf("%s ", positionals[i].c_str());
      }
      printf("\n\n");
    }
    usage();
    printf ( "\noptions are: \n" );
    for ( int i = 0; i < options.size(); i++ ) {
      printf( "%s=%s\n", options[i]->name.c_str(), options[i]->c_str());
    }
    if (this->size()) {
      printf ( "\nPositionals are:\n" );
      for ( int i = 0; i < this->size(); i++) {
	printf( " %s\n", (*this)[i].c_str() );
      }
      printf( "\n" );
    }
    if (this->unknowns.size()) {
      printf ( "\nUnknowns are:\n" );
      for ( int i = 0; i < this->unknowns.size(); i++) {
	printf( " %s\n", this->unknowns[i].c_str() );
      }
      printf( "\n\n" );
    }
    // printf ( "\n\n" );
  };
};

struct testParameters_t : parameters_t {
  bool myBool;
  int myInt;
  double myFloat;
  string myString;
  char *myChars;
  
  testParameters_t() {
    options.push_back ( new parameter_t < bool > ( &(this->myBool), "myBool", 'b', (char*) "bool", false, "help bool" ));
    options.push_back ( new parameter_t < int > ( &(this->myInt), "myInt", 'i', (char*) "int", 0, "help int" ));
    options.push_back ( new parameter_t < double > ( &(this->myFloat), "myFloat", 'f', (char*) "float", 0.0, "help float" ));
    options.push_back ( new parameter_t < string > ( &(this->myString), "myString", 'S', (char*) "string", "", "help for myString." ));
    options.push_back ( new parameter_t < char* > ( &(this->myChars), "myChars", 's', (char*) "chars", (char *) "", "help for myChars" ));

    /* printf("After init\n"); this->usage(); */
    /* printf("myBool = %d\n", myBool ); */
    /* printf("myInt = %d\n", myInt ); */
    /* printf("myFloat = %f\n", myFloat ); */
    /* printf("myString = %s\n", myString.c_str() ); */
    /* printf("myChars = %s\n", myChars ); */

  };
};

#endif //  _ARGV2OBJ_H_
