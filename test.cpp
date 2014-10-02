#include "argv2obj.h"

#include <vector>

struct AbstractField{
  virtual ~AbstractField() = 0;
};

template<class T,int fieldTypeId>
class Field: public AbstractField{
  private:
    T field;
  public:
    const static int field_type;
  public:
    virtual ~Field(){}
};

class Database_Record{
  std::vector<AbstractField*> record; 
  public:
    ~Database_Record(){
      //delete all AbstractFields in vector
    }
};


struct C {
  int a, b;
  // C(int x, int y) { a=x, b=y; };
  // C(int x, int y, int z) { a = x+z; b = y+z; };
};



int main ( int argc, const char** argv ) {
  // vector <C> v = { {1,2}, {3,4} };
  parameters_t args ( argc, argv );
}
