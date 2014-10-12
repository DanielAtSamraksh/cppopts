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


template< class T >
const string str ( T x ) {
  stringstream s;
  s << x;
  return s.str();
};
		   
template< class T >
const string str ( vector < T > v ) {
  stringstream s;
  if ( v.size() == 0 ) s << "[]";
  else {
    s << "[" << str ( v[0] );
    for ( unsigned i = 1; i < v.size(); i++)
      s << ", " << str ( v[i] );
    s << "]";
  }
  return s.str();
};


/// print out the type of a variable
const string typestr ( const bool x)    { return "bool"; };
const string typestr ( const int x)     { return "int"; };
const string typestr ( const unsigned x)     { return "unsigned"; };
const string typestr ( const double x)  { return "double"; };
const string typestr ( const string x)  { return "string"; };
const string typestr ( const char x)   { return "char"; };
const string typestr ( const char* x)   { return "char*"; };
const string typestr ( const vector< bool > v ) {
  return "vector<bool>";
};
const string typestr ( const vector< int > v ) {
  return "vector<int>";
};
const string typestr ( const vector< double > v ) {
  return "vector<double>";
};
const string typestr ( const vector< unsigned > v ) {
  return "vector<unsigned>";
};
const string typestr ( const vector< char* > v ) {
  return "vector<char*>";
};
const string typestr ( const vector< string > v ) {
  return "vector<string>";
};

int min (int i, int j) { return i < j? i: j; };

bool startswith ( const string s1, const char* s2) { 
  return startswith ( s1.c_str(), s2 ); 
};

/* The second argument starts with the first argument. */
bool startswith ( const char* s1, const char* s2 ) {
  unsigned n = strlen( s1 ); 
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
bool eq ( T x, T y ) { return x == y; };

bool atoval ( const char *s, int &i ) {
  // printf("converting %s to int %d\n", s, atoi(s));
  i = atoi ( s );
  return true;
};

bool atoval ( const char *s, unsigned &i ) {
  // printf("converting %s to int %d\n", s, atoi(s));
  i = (unsigned) atoi ( s );
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

bool atoval ( const char *s, vector <string> &v ) {
  unsigned len = strlen ( s );
  const char *start = s;
  unsigned i;
  for ( i = 0; i < len; i++ ) {
    if ( ' ' == s[i] ) {
      v.push_back ( string ( start, s+i ));
      start = s+i;
    }
  }
  v.push_back ( string ( start, s+i ));
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
  string shorts;   // short arguments
  vector <string> longs;   // long argument;
  string help;     // help text
  bool required; // true if this is required, not used in this class, but in parameters_t.
  bool is_set; // true if this value has been set.
  
  //  virtual ~abstractParameter_t() = 0;
  virtual bool parse ( unsigned &i, unsigned &j, unsigned argc, const char **argv ) = 0;
  virtual const string str() = 0;
  virtual const string typestr() = 0;
  virtual const char* c_str() = 0;
  virtual const char* usage() = 0;
  ~abstractParameter_t(){};
};


/// Class to make populating the choice vector very easy.
template < class T >
struct choices_t {
  vector < T > *v;
  choices_t () {};
  choices_t ( vector <T> * _v ) {
    this->v = _v;
  };
  choices_t &operator() ( T x ) {
    if ( ! v ) {
      printf("choices_t error: v not initialized\n");
      exit (1);
    }
    //    printf("pushing %s on %s \n", str(x).c_str(), str(v).c_str() );
    v->push_back( x );
    //    printf("pushed %s on %s \n\n", str(x).c_str(), str(v).c_str() );
    return *this;
  };
};
    
template < class T >
struct parameter_t: public abstractParameter_t {
 public:

  T defaultValue; 
  T value;
  T *valuePtr;
  vector < T > *choices;
  bool choices_should_be_freed;
  choices_t<T> choicesHelper;

   parameter_t () {};
  parameter_t ( string name, string help ) {
    this->name = name;
    this->help = help;
  };

  parameter_t<T>& addPtr ( T* p ) {
    this->valuePtr = p;
    return *this;
  };
  parameter_t<T>& addName ( string n ) { this->name = n; return *this; };
  parameter_t<T>& addHelp ( string h ) { this->help = h; return *this; };
  parameter_t<T>& addShort ( char s ) { this->shorts += s; return *this; };
  // parameter_t<T>& addLong ( char* l ) { this->longs.push_back( (string) l ); return *this; };
  parameter_t<T>& addLong ( string l ) { this->longs.push_back(l); return *this; };
  parameter_t<T>& addDefaultValue ( T v ) {
    this->defaultValue = v;
    this->value = v;
    if (this->valuePtr) *(this->valuePtr) = v;
    return *this;
  };
  parameter_t<T>& addChoice ( T c ) {
    if ( ! this->choices ) {
      this->choices = new vector<T>;
      this->choices_should_be_freed = true;
    }
    this->choices->push_back ( c );
    return *this;
  };
  parameter_t<T>& addChoices ( vector<T>* v ) {
    this->choices = v;
    return *this;
  };

  // virtual ~parameter_t(){};
  ~parameter_t(){
    if ( this->choices_should_be_freed )
      delete choices;
  };

  choices_t<T> & setChoices() {
    if ( ! this->choices ) {
      this->choices = new vector <T>;
      this->choices_should_be_freed = true;
    }
    this->choicesHelper.v = this->choices;
    
    return this->choicesHelper;
  };
  choices_t<T> & setChoices(T x) {
    return this->setChoices()(x);
  };
  
  
  string _str;

  const string str() { 
    stringstream s; s << ::str ( value ); 
    this->_str = s.str(); // save the returned string 
    return this->_str; // return the saved copy
  };

  const string typestr (){ return ::typestr ( this->value );};
  
  const char *c_str() {
    return this->str().c_str();
  };

  string _usageString;
  // may be overwritten by subclass.
  const char* usage() { return this->_usage(); }; 
  const char* _usage() {
    stringstream s;

    // name
    s << this->name << "\t";

    // options
    bool firstOption = true;
    unsigned k;
    for ( k = 0; k < this->shorts.size(); k++ ) {
      if ( firstOption ) firstOption = false;
      else s << "|";
      s << "-" << this->shorts[k];
    }
    for ( k = 0; k < this->longs.size(); k++ ) {
      if ( firstOption ) firstOption = false;
      else s << "|";
      s << "--" << this->longs[k];
    }
    
    // help
    s << "\t" << this->help;

    // default value
    s << " Default value = " << ::str ( this->defaultValue ) << ".";

    // choices
    if ( this->choices && this->choices->size() > 0 ) {
      s << " Choices = " << ::str ( this->choices->at(0) );
      for (unsigned i=1; i < this->choices->size(); i++) {
    	s << ", " << ::str ( this->choices->at(i) );
      }
      s << ".";
    }

    // save in class member so that string won't go out of scope.
    _usageString = s.str();
    return _usageString.c_str();
  };

  /// Use a char* to set the value.
  bool set ( const char* s ) {
    if (! atoval(s, this->value) ) return false;
    if (this->valuePtr) *(this->valuePtr) = this->value;
    this->is_set = true;
    if ( this->choices ) {
      this->is_set = false;
      for ( unsigned i = 0; i < this->choices->size(); i++ )
	if ( eq ( this->choices->at(i), this->value )) {
	  this->is_set = true;
	}
      if ( false == this->is_set ) {
	stringstream strm;
	strm << "Value for option " << this->name <<
	  " is not one of the valid choices (";
	
	strm << ::str ( this->choices->at(0) );
	for ( unsigned i = 1; i < this->choices->size(); i++ ) {
	  strm << ", " << ::str ( this->choices->at(i) );
	}
	strm << ").";
	printf( "%s\n", strm.str().c_str() );
      }
    }
    return this->is_set;
  };
  
  bool set ( const string s ) { return this->set ( s.c_str() ); };
  
  /// Parse argv starting at argv[i][j]. Return false on error. Update
  /// i and j as needed.
  bool parse ( unsigned &i, unsigned &j, unsigned argc, const char **argv ) {
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
  bool parselong ( unsigned &i, unsigned &j, unsigned argc, const char **argv ) {
    // we start pointing at the long
    // printf("checking %s, parsing long,  %s %s\n", this->longv[0], argv[i]+j, argv[i]);
    // printf("i = %d, j = %d\n", i, j);
    for ( unsigned k = 0; k < this->longs.size(); k++ ) {
      const char* o = this->longs[k].c_str(); 
      if ( startswith ( o, argv[i]+2 ) ) {
	char char_after = argv[i][ 2 + strlen(o) ];
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
	  if (! this->set( argv[i]+2+strlen(o)+1 )) return false;
	  i += 1; j = 0;
	  return true;
	default: ; // is longer than what we're looking for
	}
      }
    }
    return true; // no match
  };

  /// returns false on error
  bool parseshort ( unsigned &i, unsigned &j, unsigned argc, const char **argv ) {
    for ( unsigned k = 0; k < this->shorts.size(); k++ ) {
      char o = this->shorts[k]; 
      // check shorts
      if (j < 1) { printf("Parse error, j <= 0\n"); exit(1); }
      // printf("type = %s\n", typeid(T).name());
      if ( o == argv[i][j] ) {
	if (! strncmp(typeid(T).name(), "bool", 1)) {
	  if (! this->set("true")) return false;
	  j++;
	}
	else if (j+1 < strlen(argv[i])) { // look for value adjacent
	  if (! this->set ( argv[i]+j+1 )) return false;
	  i++; j = 0;
	}
	else if ( i + 1 < argc ) { // look for value in next arg
	  if (! this->set ( argv[i+1] )) return false;
	  *(this->valuePtr) = this->value;
	  i += 2; j=0;
	}
	else {
	  printf ("Parse error: missing value for short option %c in %s\n",
		  o, argv[i]);
	  i++; j=0; // avoid infinite loop if return value is not checked.
	  return false;
	}
	return true;
      }
    }
    return true; // no match
  };

};


/* template < class T > const string parameter_t < vector < T >>::typestr() { */
/*   stringstream s; */
/*   s << "vector <" <<  */
/*   return "test"; }; */


/* template <> const string parameter_t < bool >::typestr() { */
/*   return "bool"; */
/* }; */
/* template <> const string parameter_t < int >::typestr() { */
/*   return "int"; */
/* }; */
/* template <> const string parameter_t < unsigned >::typestr() { */
/*   return "unsigned"; */
/* }; */
/* template <> const string parameter_t < double >::typestr() { */
/*   return "double"; */
/* }; */
/* template <> const string parameter_t < string >::typestr() { */
/*   return "string"; */
/* }; */
/* template <> const string parameter_t < char >::typestr() { */
/*   return "char"; */
/* }; */
/* template <> const string parameter_t < char* >::typestr() { */
/*   return "char*"; */
/* }; */
/* template <> const string parameter_t < vector <char* >::typestr() { */
/*   return "char*"; */
/* }; */


class parameters_t {
 public:

  parameters_t () {};

  // Use a vector to store options to keep them in order.  Name lookup
  // is linear search which is ok if number of parameters are small.
  vector < abstractParameter_t* > options;
  vector < string > positionals, unknowns; // the part of the argvs
					   // not parsed as options
  vector < char* > argv; // after parsing, these point to the args
			 // after --
  vector < char > argsbuffer; // buffer used to store char[] pointed
			      // to by argv
  // if passing the args after the -- to another function expecting
  // arguments (int argc, char** argv), pass this->argv.size() for
  // argc and this->argv.data() for argv.

  unsigned size() { return positionals.size(); };

  string operator[] ( unsigned i ) { return positionals[i]; };

  abstractParameter_t* operator[] ( string n ) {
    for ( unsigned i = 0; i < options.size(); i++) {
      if ( n == options[i]->name ) return options[i];
    }
      printf( "Lookup error: Parameter %s not found.\n", n.c_str() );
      exit(1);
  };

  string _usage;
  string usage() {
    stringstream s; 
    for ( unsigned i = 0; i < options.size(); i++ ) {
      s << options[i]->usage() << "\n\n";
      // printf("%s\n\n", options[i]->usage());
    };
    _usage = s.str();
    return _usage;
  };

  // return false on failure.
  bool parse (unsigned argc, const char **argv) {
    unsigned i=1, j=0;
    unsigned oldi, oldj;
    do { do {
      oldi = i; oldj = j;
      for ( unsigned k = 0; k < options.size(); k++ ) {
	// printf( "  at %s looking for %s\n", argv[i], options[k]->name.c_str() );
	if (! options[k]->parse ( i, j, argc, argv )) { // error
	  printf("Parse error on %s.\n  Argv = ", argv[i]);
	  for ( unsigned k = 0; k < argc; k++ ) {
	    if (k) printf(" ");
	    printf ( "%s", argv[k] );
	  }
	  printf("\n\n");
	  return false;
	}
      }} while ( oldi != i || oldj != j );
      // we're stuck, find out why
      if ( i >= argc ) break; // done with args
      if ( eq ( "--", argv[i] )) { // end of options, parse positionals
	for ( i++; i < argc; i++ ) {
	  positionals.push_back(argv[i]);
	  // push this arg, byte-by-byte, into argsbuffer
	  j=0;
	  do { this->argsbuffer.push_back(argv[i][j]); }
	  while (argv[i][j++] != '\0');
	}
	// argv gets the pointers to the args in the argsbuffer.
	// first pointer is to the beginning
	this->argv.push_back ( this->argsbuffer.data() );
	for ( j = 1; j < argsbuffer.size(); j++ )
	  // the rest of the pointers are preceded by '\0'
	  if ( '\0' == *(this->argsbuffer.data() + j - 1))
	    this->argv.push_back ( this->argsbuffer.data() + j );
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
	printf("error parsing at %s (argument %d), expected the beginning of a positional\n",
	       argv[i], i);
	return false;
      }
      // we've reached a positional argument. Consume it and continue.
      positionals.push_back ( argv[i] ); i++; continue;
    } while (1); //
    // check if all the required parameters were set.
    for ( unsigned k = 0; k < this->options.size(); k++ ) {
      abstractParameter_t *o = options[k];
      if ( o->required && ! o->is_set ) {
	printf( "Parse error. Required parameter %s has not been set.\n",
		o->name.c_str() );
	return false;
      }
    }
    return true;
  };
  string _dumpString;
  string dump() {
    stringstream s;
    for ( unsigned i = 0; i < options.size(); i++ ) {
      abstractParameter_t *o = options[i];      

      // The option pointer must be dynamically cast a particular type
      // of parameter_t so we can get at it's value.
      s << o->name << "= (" << o->typestr() << ") " << o->str() << "\n";
      
      /* if ( parameter_t<int> *p = dynamic_cast < parameter_t<int> * > ( o )) */
      /* 	s << p->name << "= (int) " << p->value << "\n"; */

      /* else if ( parameter_t<double> *p = dynamic_cast <parameter_t <double>*> ( o )) */
      /* 	s << p->name << "= (double) " << p->value << "\n"; */

      /* else if ( parameter_t<bool> *p = dynamic_cast<parameter_t <bool>*> ( o )) */
      /* 	s << p->name << "= (bool) " << p->value << "\n"; */

      /* else if ( parameter_t<char*> *p = dynamic_cast<parameter_t <char*>*> ( o )) */
      /* 	s << p->name << "= (char*) " << p->value << "\n"; */

      /* else if ( parameter_t<string> *p = dynamic_cast<parameter_t <string>*> ( o )) */
      /* 	s << p->name << "= (string) " << p->value << "\n"; */

    }
    for ( unsigned i = 0; i < positionals.size(); i++ ) {
      s << i << "=" << positionals[i] << "\n";
    }

    for ( unsigned i = 0; i < unknowns.size(); i++ ) {
      s << "unknown option " << i << "=" << unknowns[i] << "\n";
    }


    this->_dumpString = s.str();
    return this->_dumpString;
  };
};

#endif //  _ARGV2OBJ_H_
