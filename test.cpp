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




int main ( int argc, const char** argv ) {
  parameters_t args ( argc, argv );
}
