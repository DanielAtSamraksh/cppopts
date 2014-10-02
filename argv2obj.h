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

struct abstractParameter_t {
  //  virtual ~abstractParameter_t() = 0;
  virtual bool parse ( int &i, int &j, const int argc, const char **argv ) = 0;
  virtual const char* c_str() = 0;
  virtual const char* usage() = 0;
  ~abstractParameter_t(){};
};

template < class T >
struct parameter_t: public abstractParameter_t {
 public:
  parameter_t () {};
  parameter_t ( char _short, char* _long ) {
    init ( _short, _long );
  };
  parameter_t ( char _short, char* _long, T def ) {
    init ( _short, _long, def );
  };
  
  parameter_t ( char _short, char* _long, T def, string help ) {
    init ( _short, _long, def, help );
  };
  
  parameter_t ( string name, char _short, char* _long, T def, string help ) {
    init ( name, _short, _long, def, help );
  };
  
  // virtual ~parameter_t(){};
  ~parameter_t(){};

  string name;
  char s;   // short argument
  char *l;   // long argument
  string help;     // help text
  T defaultValue; 
  T value;

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

  void init ( string name, char _short, char* _long, T def, string help ) {
    this->init ( _short, _long);
    this->value = def;
    this->defaultValue = def;
    this->help = help;
    this->name = name;
  };
  
  void init ( char _short, char* _long, T def, string help ) {
    this->init ( _short, _long);
    this->value = def;
    this->defaultValue = def;
    this->help = help;
  };
  
  void init ( char _short, char* _long, T def) {
    this->init ( _short, _long);
    this->value = def;
    this->defaultValue = def;
  };
  
  void init ( char _short, char* _long ) {
    this->s = _short;
    this->l = _long;
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
    return false;
  };
  
  bool parseshort ( int &i, int &j, const int argc, const char **argv ) {
    // check shorts
    if (j < 1) { printf("parse error, j <= 0\n"); exit(1); }
    // printf("type = %s\n", typeid(T).name());
    if ( this->s == argv[i][j] ) {
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
	printf ("missing value for short option %c in %s\n", this->s, argv[i]);
	i++; j=0; // avoid infinite loop if return value is not checked.
	return true;
      }
      return false;
    }
    return false;
  };

};


typedef map < string, abstractParameter_t* > pmap_t;

struct abstractParameterSpec_t {
  string name;
  char shortOpt;
  string longOpt;
  string help;
  virtual abstractParameter_t *toParameter()=0;
};

template < class T >
struct parameterSpec_t: abstractParameterSpec_t {
  T defaultValue;
  parameter_t <T> *toParameter() {
    new parameter_t <T> ( shortOpt, longOpt, defaultValue, help );
  };
};
  
class parameters_t {
 public:

  parameters_t () {};
  parameters_t ( int argc, const char ** argv ) { this->parse(argc, argv); };
  pmap_t params;

  abstractParameter_t* operator[] ( string n ) { return params[n]; };

  template < class T >
  bool add( parameterSpec_t < T > *p ) {
    params[p->name]= p->toParameter(); // new parameter_t < T > ( p.shortOpt, p.longOpt, p.defaultValue, p.help );
    return false;
  };
  bool add ( vector < abstractParameterSpec_t* > ps ) {
    for ( vector < abstractParameterSpec_t* > :: iterator it = ps.begin(); it != ps.end(); it++) {
      params[(*it)->name] = (*it)->toParameter();
    }
    return false;
  };

  string _usage;
  string usage() {
    stringstream s; 
    for ( pmap_t::iterator it = params.begin(); it != params.end(); it++ ) {
      s << it->second->usage();
      printf("%s\n", it->second->usage());
    };
    _usage = s.str();
    return _usage;
  };

  void parse (int argc, const char **argv) {
    
    params["myBool"] = new parameter_t < bool > ("myBool", 'b', (char*) "bool", false, "help bool"); 
    params["myInt"] = new parameter_t < int > ("myInt", 'i', (char*) "int", 0, "help int"); 
    params["myFloat"] = new parameter_t < double > ("myFloat", 'f', (char*) "float", 0.0, "help float"); 
    params["myString"] = new parameter_t < string > ("myString", 'S', (char*) "string", "", "help for myString."); 
    params["myChars"] = new parameter_t < char* > ("myChars", 's', (char*) "chars", (char *) "", "help for myChars"); 

    // p["myChars"] = &myChars;
    
    int i=1, j=0;
    int oldi, oldj;
    do {
      oldi = i; oldj = j;
      for ( pmap_t::iterator it = params.begin(); it != params.end(); it++) {
	it->second->parse ( i, j, argc, argv );
      }
    } while ( oldi != i || oldj != j );

    // check
    
    usage();
    printf ( "\nparameters are: \n" );
    for ( pmap_t::iterator it = params.begin(); it != params.end(); it++) {
      printf( "%s=%s\n", it->first.c_str(), it->second->c_str());
    }
    printf ( "\n\n" );
  };

};

#endif //  _ARGV2OBJ_H_
