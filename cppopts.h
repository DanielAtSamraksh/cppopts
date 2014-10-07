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

/// boolean vectors are optimized so the template definition doesn't work.
bool eq ( const bool x, const bool y ) { return x == y; };

template < class T >
bool eq ( T x, T y ) { return x == y; }

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
  bool nolong, noshort; // true if there is no long or short defined
  string help;     // help text

  //  virtual ~abstractParameter_t() = 0;
  virtual bool parse ( int &i, int &j, const int argc, const char **argv ) = 0;
  virtual const string str() = 0;
  virtual const char* c_str() = 0;
  virtual const char* usage() = 0;
  ~abstractParameter_t(){};
};

template < class T >
struct parameter_t: public abstractParameter_t {
 public:

  parameter_t ( T * ptr, string name, char _short, char* _long,
		T def, string help ) {
    init ( ptr, name, _short, _long, def, help );
  };

  parameter_t ( T * ptr, string name, char _short, char* _long,
		T def, string help, vector<T> choices ) {
    init ( ptr, name, _short, _long, def, help, choices );
  };

  // no short
  parameter_t ( T * ptr, string name, char* _long,
		T def, string help ) {
    init ( ptr, name, _long, def, help );
  };

  parameter_t ( T * ptr, string name, char* _long,
		T def, string help, vector<T> choices ) {
    init ( ptr, name, _long, def, help, choices );
  };

  // no long
  parameter_t ( T * ptr, string name, char _short,
		T def, string help ) {
    init ( ptr, name, _short, def, help );
  };

  parameter_t ( T * ptr, string name, char _short,
		T def, string help, vector<T> choices ) {
    init ( ptr, name, _short, def, help, choices );
  };

  // virtual ~parameter_t(){};
  ~parameter_t(){};

  T defaultValue; 
  T value;
  T *valuePtr;
  vector < T > choices;

  string _str;

  const string str() { 
    stringstream s; s << value; 
    this->_str = s.str(); // save the returned string 
    return this->_str; // return the saved copy
  };

  const char *c_str() {
    return this->str().c_str();
  };

  string _usageString;
  // may be overwritten by subclass.
  const char* usage() { return this->_usage(); }; 
  const char* _usage() {
    stringstream s;
    s << "name: " << name << ", short: -" << this->s << ", long: --" << l
      << ", defaultValue: " << defaultValue << ", value: " << value;
    if (choices.size()) {
      s << ", choices = [";
      for (int i=0; i<choices.size(); i++) {
	if (i) s << ", "; // comma separator
	s << choices[i];
      }
      s << "]";
    }
    s  << ", help: " << help << "\n";
    _usageString = s.str();
    return _usageString.c_str();
  };

  // all options
  void init ( T* ptr, string name, char _short, char* _long,
	      T def, string help, vector<T> choices ) {
    this->s = _short;
    this->l = _long;
    this->nolong = this->noshort = false;
    this->choices = choices;
    this->init ( ptr, name, def, help );
  };
  // no short
  void init ( T* ptr, string name, char* _long,
	      T def, string help, vector<T> choices ) {
    this->l = _long;
    this->noshort = true;
    this->nolong = false;
    this->choices = choices;
    this->init ( ptr, name, def, help );

    this->choices = choices;
    this->init ( ptr, name, def, help );
  };
  // no long
  void init ( T* ptr, string name, char _short,
	      T def, string help, vector<T> choices ) {
    this->s = _short;
    this->noshort = false;
    this->nolong = true;
    this->choices = choices;
    this->init ( ptr, name, def, help );
  };
  // no choices
  void init ( T* ptr, string name, char _short, char* _long,
	      T def, string help ) {
    this->s = _short;
    this->l = _long;
    this->noshort = false;
    this->nolong = false;
    this->init ( ptr, name, def, help );
  };
  // no choices, no short
  void init ( T* ptr, string name, char* _long,
	      T def, string help ) {
    this->l = _long;
    this->noshort = true;
    this->nolong = false;
    this->init ( ptr, name, def, help );
  };
  // no choices, no long
  void init ( T* ptr, string name, char _short,
	      T def, string help ) {
    this->s = _short;
    this->noshort = false;
    this->nolong = true;
    this->init ( ptr, name, def, help );
  };
  // common case
  void init ( T* ptr, string name, T def, string help ) {
    if (ptr) this->valuePtr = ptr;
    else this->valuePtr = 0;
    this->value = def;
    if (this->valuePtr) *(this->valuePtr) = def;
    this->defaultValue = def;
    this->help = help;
    this->name = name;
  };

  bool set ( const char* s ) {
    if (! atoval(s, this->value) ) return false;
    if (this->valuePtr) *(this->valuePtr) = this->value;
    if ( this->choices.size() ) {
      for ( int i = 0; i < this->choices.size(); i++ )
	if ( eq ( this->choices[i], this->value )) return true;
      stringstream strm;
      strm << "Value for option " << this->name <<
	" is not one of the valid choices (";
      for ( int i = 0; i < this->choices.size(); i++ ) {
	if ( i ) strm << ", ";
	strm << this->choices[i];
      }
      strm << ").";
      printf( "%s\n", strm.str().c_str() );
      return false;
    }
    return true;
  };
  
  bool set ( const string s ) { return this->set ( s.c_str() ); };
  
  /// Parse argv starting at argv[i][j]. Return false on error. Update
  /// i and j as needed.
  bool parse ( int &i, int &j, const int argc, const char **argv ) {
    // i = index into argv, j = index into argv[i]
    if ( i >= argc) return true; // no more arguments
    if ( j > 0 && j >= strlen ( argv[i] )) { 
      i++; j = 0; // move to next argument if at the end argv[i]
      if ( i >= argc) return true; // return if no more arguments
    }
    if ( '-' == argv[i][0] ) {
      if ('-' == argv[i][1]) {
	// -- means stop processing
	if ('\0' == argv[i][2]) return true;
	j=2;
	return this->parselong ( i, j, argc, argv );
      }
      if (j < 1) j = 1;
      // printf("parsing short i=%d j=%d %c\n", i, j, argv[i][j]);
      return parseshort ( i, j, argc, argv );
    }
    return true; // not long or short option, just return
  };

  /// returns false on error
  bool parselong ( int &i, int &j, const int argc, const char **argv ) {
    // we start pointing at the long
    // printf("checking %s, parsing long,  %s %s\n", this->longv[0], argv[i]+j, argv[i]);
    // printf("i = %d, j = %d\n", i, j);
    if ( this->nolong ) return true;
    if ( startswith ( this->l, argv[i]+2 ) ) {
      char char_after = argv[i][ 2 + strlen(l) ];
      switch ( char_after ) {
      case '\0': // value in next argument
	// printf("type = %s\n", typeid(T).name());
	if (! strncmp(typeid(T).name(), "bool", 1)) {
	  if (! this->set("true") ) return false;
	  i++; j = 0;
	}
	else if ( i+1 >= argc ) {
	  printf("Missing argument for option %s\n", argv[i]);
	  i++; j=0;// avoid infinite loop
	  return false;
	}
	else {
	  if ( ! this->set( argv[i+1] )) return false;
	  i += 2; j = 0;
	}
	return true; 
      case '=': // value in this argument
	if (! this->set( argv[i]+2+strlen(l)+1 )) return false;
	i += 1; j = 0;
	return true;
      default: ; // is longer than what we're looking for
      }
    }
    return true;
  };

  /// returns false on error
  bool parseshort ( int &i, int &j, const int argc, const char **argv ) {
    if ( this->noshort ) return true;
    // check shorts
    if (j < 1) { printf("parse error, j <= 0\n"); exit(1); }
    // printf("type = %s\n", typeid(T).name());
    if ( this->s == argv[i][j] ) {
      if (! strncmp(typeid(T).name(), "bool", 1)) {
	if (! this->set("true")) return false;
	j++;
      }
      else if (j+1 < strlen(argv[i])) {
	if (! this->set ( argv[i]+j+1 )) return false;
	i++; j = 0;
      }
      else if ( i + 1 < argc ) {
	if (! this->set ( argv[i+1] )) return false;
	*(this->valuePtr) = this->value;
	i += 2; j=0;
      }
      else {
	printf ("missing value for short option %c in %s\n", this->s, argv[i]);
	i++; j=0; // avoid infinite loop if return value is not checked.
	return false;
      }
      return true;
    }
    return true;
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

  // return false on failure.
  bool parse (int argc, const char **argv) {
    int i=1, j=0;
    int oldi, oldj;
    do { do {
      oldi = i; oldj = j;
      for ( int k = 0; k < options.size(); k++ ) {
	// printf( "  at %s looking for %s\n", argv[i], options[k]->name.c_str() );
	if (! options[k]->parse ( i, j, argc, argv )) { // error
	  printf("Parse error.\n");
	  return false;
	}
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
	return false;
      }
      // we've reached a positional argument. Consume it and continue.
      positionals.push_back ( argv[i] ); i++; continue;
    } while (1); //
    return true;
  };
  string _dumpString;
  string dump() {
    stringstream s;
    for ( int i = 0; i < options.size(); i++ ) {
      abstractParameter_t *o = options[i];      

      // The option pointer must be dynamically cast a particular type
      // of parameter_t so we can get at it's value.
      if ( parameter_t<int> *p = dynamic_cast < parameter_t<int> * > ( o ))
	s << p->name << "= (int) " << p->value << "\n";

      else if ( parameter_t<double> *p = dynamic_cast <parameter_t <double>*> ( o ))
	s << p->name << "= (double) " << p->value << "\n";

      else if ( parameter_t<bool> *p = dynamic_cast<parameter_t <bool>*> ( o ))
	s << p->name << "= (bool) " << p->value << "\n";

      else if ( parameter_t<char*> *p = dynamic_cast<parameter_t <char*>*> ( o ))
	s << p->name << "= (char*) " << p->value << "\n";

      else if ( parameter_t<string> *p = dynamic_cast<parameter_t <string>*> ( o ))
	s << p->name << "= (string) " << p->value << "\n";

    }
    for ( int i = 0; i < positionals.size(); i++ ) {
      s << i << "=" << positionals[i] << "\n";
    }

    for ( int i = 0; i < unknowns.size(); i++ ) {
      s << "unknown option " << i << "=" << unknowns[i] << "\n";
    }


    this->_dumpString = s.str();
    return this->_dumpString;
  };
};

#endif //  _ARGV2OBJ_H_
