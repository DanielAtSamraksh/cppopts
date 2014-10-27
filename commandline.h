
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


// todo:  long and short, parse, choices, map (no duplicate names)

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
    t;
};


class opts_t {

 public:
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
      cout << "Error: can't add choice because of a type mismatch.\n";
      cout << "Choice " << c << " has type " << typestr ( c )
	   << "but option " << opts[last] << " has type " << opts[last]->type() << "\n";
      exit ( 1 );
    }
    opt->addChoice ( c );
    return *this;
  };


  bool parse ( int argc, char *argv[] ) {
    for ( unsigned i = 0 ; i < argc ; i++ ) {
      char* value = strchr ( argv[i], '=' ); 
      string name = value? string(argv[i], value): argv[i]; 
      if (value) value += 1; // move value off the '='

      bool ok = false;
      for ( unsigned j = 0; j < opts.size(); j++ ) {
	opt_t *o = opts[j];
	if ( o->name == name ) {
	  if ( value ) {
	    ok = o->parse ( value );
	  }
	  else if ( o->isflag ()) {
	    ok = o->parse ((char*) "true" );
	  }
	  else if ( i + 1 < argc ) {
	    ok = o->parse ( argv[++i] );
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
    return true;
  };

  string usage() {
    stringstream s;
    for ( unsigned i = 0; i < opts.size(); i++ ) {
      s << opts[i]->usage() << "\n";
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
