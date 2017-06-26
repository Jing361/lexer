#include"variable.hh"

using namespace std;

variable::variable( const string& name ):
  mName( name ){
}

string variable::generate() const{
  //TODO variables have 2 primary uses.  read and write.
  // how to discern between them?
  return "PLACEHOLDER\n";
}

