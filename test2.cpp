#include "cppopts.h"

int main ( int argc, char** argv ) {
  vector <int> *v = new vector<int>;
  v->push_back(0);
  choices_t <int> c(v);
  c
    (1) // push 1 and return to push again
    (2) // push 2
    (3);
  printf ("v = %s, size = %lu \n",
	  str ( *(c.v) ).c_str(), c.v->size() );
};
