
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
	cout << "Can't find opt name " << name << "\n";
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
    opt_t ( string n, string h ): name(n), help(h) {};

  public:
    string name;
    string help;
    virtual bool isflag() = 0; // boolean flags are handled differently; they don't have values
    virtual bool parse ( char* ) = 0;
    virtual string usage() = 0;
    virtual string str() = 0;

  };

  template < class T >
    class opt_basictype_t: public opt_t {
  public:
    opt_basictype_t ( string n, string h, T* v ): opt_t(n, h), value(v), defaultValue(*v) {};
    T *value;
    T defaultValue;

    string usage() {
      stringstream s;
      s << this->name << "\n  " << this->help << "\n"
	<< "  default value (" << this->type() << ") " << defaultValue << "\n";
      return s.str();
    };

    string str() {
      stringstream s;
      s << "value (" << this->type() << ") " 
	<< *(this->value) << ", default " << this->defaultValue << "\n";
      return s.str();
    };

    string type() {
      string t = typeid( T ).name();
      return "i" == t? "int":
	"f" == t? "double":
	t;
    };

    bool parse( char* v ) {
      stringstream s;
      s << v; 
      s >> *(this->value);
      cout << ( ! s.bad()? "successfully ": "unsuccessfully " )
	   << "parsed " << name << " " << this->str() << "\n";
      return ! s.bad();
    };
    bool isflag () { return false; };
  };

  typedef vector < opt_t* > optsV_t;
  optsV_t opts; 

};

template <> bool opts_t::opt_basictype_t < bool>::
isflag () { return true; };

template <> bool opts_t::opt_basictype_t < bool>::
parse ( char* v ) { 
  if ( strncmp ((char*) "true", v, strlen ( v )) == 0 || 
       strncmp ((char*) "TRUE", v, strlen ( v )) == 0 || 
       strncmp ((char*) "1", v, strlen ( v )) == 0 ) {
    *(this->value) = true;
  }
  else if ( strncmp ((char*) "false", v, strlen ( v )) == 0 || 
	    strncmp ((char*) "FALSE", v, strlen ( v )) == 0 ||
	    strncmp ((char*) "1", v, strlen ( v )) == 0 ) {
    *(this->value) = false;
  }
  else {
    cout << "Not a bool value: " << v; 
    return false;
  }
  return true; 
};
