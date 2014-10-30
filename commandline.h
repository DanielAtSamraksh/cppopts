
#include <sstream> // stringstream
#include <iostream> // cout
#include <vector> // vector type
#include <map> // map type
#include <string> // string type
#include <typeinfo> // typeid
#include <cstring> // strchr, strncmp
#include <cstdlib> // exit
using std::cout;
using std::map;
using std::vector;
using std::stringstream;
using std::string;

/// Simple command-line parser modeled after the ns3 CommandLine class. 
///
/// Add values to initialize by calling .addValue( string name, T &value ) 
/// where T is the type of value. This will configure the parser set the variable
/// "value" when it encounters "name". 
/// For example: ```opts.addValue( "--count", &count ); opts.parse(argc, argv); ``` 
/// will set the variable count to 4 when parsing "--count=4" or "--count 4". 
///
/// The methods "addValue" and "addChoice" return the parser, so they can be strung
/// together. See the example program below.
/// 
/// The "parse ( int argc, char *argv[] )" method returns true if it
/// parses with no errors. It is an error to encounter an unknown option.
/// 
/// Parsing storts from argv[0], so if you're calling parse directly from main,
/// you'll probably want to skip the first argv (which contains the program name.)
/// Eg: args.parse(argc-1, argv+1)
/// 
/// Parsing stops when a double dash (--) is encountered and the argc and argv members 
/// are set where parsing left off.

#ifdef _example_use_ // this is a documenting example
#include "commandline.h"

using std::stringstream;
using std::cout;

int main ( int argc, char** argv ) {

  int x=1, y=2;  // declare two ints with default values
  float pi=3.14; // declare float with default value
  string str="default";    // declare string with default value

  opts_t opts; // declare an opts_t instance and configure it.
  opts
    .addValue("x", "Help string for x value", &x) // add option for int x
    .addValue("y", "Help string for y value", &y) // add option for int y
    .addChoice( 1 )  // add choices for y (the last option)
    .addChoice( 2 )
    .addChoice( 3 )
    .addValue("pi", "Help string for pi value", &pi)
    .addValue("string", "Help string for string value", &str);

  cout << "Usage:\n" << argv[0] << " [options]\n" 
       << opts.usage() << "\n\n";

  cout << "Parsing arguments to " << argv[0] << "\n\n";
  // Note that parsing starts from argv[0], so call parse on arc-1 and argv+1.
  if ( ! opts.parse ( argc-1, argv+1 ) ) {
    cout << "error parsing\n";
    cout << opts.usage();
  }
  
  cout << "\nDumping values before exit:\n" << opts.dump();

};
#endif //  example

// todo:  long and short

/// output the name of a variable's type.
template < class T >
string typestr ( T v ) {
  string t = typeid( v ).name();
  return typeid( int ).name() == t?      "int":
    typeid( unsigned ).name() == t?      "unsigned":
    typeid( unsigned long ).name() == t? "unsigned long":
    typeid( long ).name() == t?          "long":
    typeid( double ).name() == t?        "double":
    typeid( float ).name() == t?         "float":
    typeid( string ).name() == t?        "string":
    typeid( char* ).name() == t?         "char*":
    typeid( char** ).name() == t?        "char**":
    t;
};


class opts_t {

 public:
  // argc and argv are set to point to the parameters after the "--"
  int argc;
  char **argv;
  
  opts_t() {
    this->argc = -1; 
    this->argv = (char **) 0;
  };

  template < class T > 
    opts_t &addValue ( string name, string help, T* v ) {
    // check for duplicates
    for ( unsigned i = 0; i < opts.size(); i++) {
      if ( name == opts[i]->name ) { 
	cout << "Error: duplicate " << name << "\n"; 
	exit ( 1 );
      }
    }
    opt_basictype_t < T > *o = new opt_basictype_t < T > ( name, help, v );
    opts.push_back ( o ); 
    return *this;
  };

  template < class T >
    opts_t &addChoice ( T c ) { 
    unsigned last = opts.size() - 1;
    if ( last < 0 ) { 
      cout << "Error: no option to add a choice to.";
      exit (1); 
    }

    opt_basictype_t < T > *opt = 
      dynamic_cast < opt_basictype_t<T>* > ( opts[ last ] );
    if ( ! opt ) {
      // type mismatch; 
      // check if it's between string and something compatible (char[])
      opt_basictype_t < string > *opt2 = 
	dynamic_cast < opt_basictype_t<string>* > ( opts[ last ] );
      if ( opt2 ) {
	opt2->addChoice ( (string) c ); 
	return *this;
      }
      cout << "Error: can't add choice because of a type mismatch.\n";
      cout << "Choice " << c << " has type " << typestr ( c )
	   << " but option " << opts[last] << " has type " << opts[last]->type() << "\n\n";
      exit ( 1 );
    }
    opt->addChoice ( c );
    return *this;
  };


  bool parse ( int argc, char *argv[] ) {
    bool ok = true;
    for ( unsigned i = 0 ; i < argc ; i++ ) {
      if ( ! strcmp ( "--", argv[i] )) {
	// stop parsing, set the argc and argv members.
	this->argc = argc-i-1; 
	this->argv = argv+i+1;
	return ok;
      }
      char* value = strchr ( argv[i], '=' ); 
      string name = value? string(argv[i], value): argv[i]; 
      if (value) value += 1; // move value off the '='

      ok = false;

      for ( unsigned j = 0; j < opts.size(); j++ ) {
	opt_t *o = opts[j];
	if ( o->name == name ) {
	  if ( value ) {
	    ok = o->parse ( value );
	    continue;
	  }
	  else if ( o->isflag ()) {
	    ok = o->parse ((char*) "true" );
	    continue;
	  }
	  else if ( i + 1 < argc ) {
	    ok = o->parse ( argv[++i] );
	    continue;
	  }
	  else {
	    cout << "Missing value for argument " << i << "(" << argv[i] << ")\n";
	    return false;
	  }
	}
      }
      if ( ! ok ) {
	cout << "Error parsing option " << name << "\n";
	return false; // not found
      }
    }
    return ok;
  };

  string usage() {
    stringstream s;
    for ( unsigned i = 0; i < opts.size(); i++ ) {
      s << opts[i]->usage() << "\n";
    }
    return s.str();
  };

  string dump() {
    stringstream s; 
    for ( unsigned i = 0; i < opts.size(); i++ ) {
      s << opts[i]->dump() << "\n";
    }
    if ( this->argc > 0 ) {
      s << "argc = " << this->argc << ", argv =\n";
      for ( unsigned i = 0; i < this->argc; i++ ) {
	s << "  [" << i << "] " << this->argv[i] << "\n";
      }
    }
    return s.str();
  };

 private:
  class opt_t {
    // http://stackoverflow.com/questions/7405740/how-can-i-initialize-base-class-member-variables-in-derived-class-constructor
  protected:
    opt_t ( string n, string h ): name(n), help(h), hasChoices(false) {};

  public:
    string name;
    string help;
    bool hasChoices;

    virtual bool isflag() = 0; // boolean flags are handled differently; they don't have values
    virtual bool parse ( char* ) = 0;
    virtual string usage() = 0;
    virtual string str() = 0;
    virtual string dump() = 0;
    virtual string type() = 0;
    virtual string parseMsg ( bool ok ) {
      stringstream s;
      s << ( ok ? "successfully ": "unsuccessfully " ) << "parsed " 
	<< "(" << this->type() << ") " 
	<< this->name << " " << this->str() << "\n";
      cout << s.str();
      return s.str();
    };
  };

  template < class T >
    class opt_basictype_t: public opt_t {

  public:
    T *value;
    T defaultValue;
    vector < T > choices;
    opt_basictype_t ( string n, string h, T* v ): opt_t(n, h), value(v), defaultValue(*v) {};

    template < class V > bool addChoice ( V x ) {
      this->choices.push_back( x );
      this->hasChoices = true;
      return true;
    };

    virtual string usage() {
      stringstream s;
      s << this->name << "\n";

      // help
      unsigned h0=0, h1;
      while ( h0 < this->help.size() ) {
	h1 = this->help.find ( '\n', h0 );
	s << "  " << this->help.substr ( h0, h1-h0 ) << "\n";
	h0 = (h1 < h1+1)? h1+1: this->help.size();
      }
      
      s << "  default value (" << this->type() << ") " << defaultValue << "\n";

      if ( this->hasChoices ) {
	unsigned nchoices = this->choices.size();
	if ( nchoices == 0 ) s << "No valid values.\n";
	else {
	  s << "  Choices are: \n";
	  for ( unsigned i = 0; i < nchoices; i++ ) {
	    s << "    " << this->choices[i] << "\n";
	  }
	}
      }
      return s.str();
    };

    virtual string str() {
      stringstream s;
      s << *(this->value);
      return s.str();
    };

    virtual string dump() {
      stringstream s;
      s << this->name << ": " << *(this->value);
      return s.str();
    };
      
    virtual string type() {
      return typestr ( this->defaultValue );
    };

    virtual bool checkChoices() {
      if ( ! this->hasChoices ) return true;
      for ( unsigned i = 0; i < this->choices.size(); i++ ) {
	if ( this->choices[i] == *(this->value) ) return true;
      }
      cout << "Bad value (" << *(this->value) << ") for option " << this->name 
	   << ". Must be one of:\n";
      for ( unsigned i = 0; i < this->choices.size(); i++ ) {
	cout << "  " << this->choices[i] << "\n";
      }
      return false;
    };

    virtual bool parse( char* v ) {
      bool ok = false;
      stringstream s;
      s << v; 
      s >> *(this->value);
      ok = ! s.bad() && this->checkChoices();
      this->parseMsg ( ok ); 
      return ok;
    };

    bool isflag () { return false; };
  };

  typedef vector < opt_t* > optsV_t;
  optsV_t opts; 

};

template <> bool opts_t::opt_basictype_t < bool >::
isflag () { return true; };

template <> bool opts_t::opt_basictype_t < bool >::
parse ( char* v ) { 
  bool ok = false;
  if ( strncmp ((char*) "true", v, strlen ( v )) == 0 || 
       strncmp ((char*) "TRUE", v, strlen ( v )) == 0 || 
       strncmp ((char*) "1", v, strlen ( v )) == 0 ) {
    *(this->value) = true;
    ok = true;
  }
  else if ( strncmp ((char*) "false", v, strlen ( v )) == 0 || 
	    strncmp ((char*) "FALSE", v, strlen ( v )) == 0 ||
	    strncmp ((char*) "1", v, strlen ( v )) == 0 ) {
    *(this->value) = false;
    ok = true;
  }
  else {
    cout << "Not a bool value: " << v; 
  }
  ok = ok && this->checkChoices();
  this->parseMsg ( ok );
  return ok; 
};

template <> bool opts_t::opt_basictype_t < string >::
parse ( char* v ) {
  bool ok = false;
  if ( v ) {
  *(this->value) = v;
  ok = true;
  }
  ok = ok && this->checkChoices();
  this->parseMsg ( ok ); 
  return ok;
};
