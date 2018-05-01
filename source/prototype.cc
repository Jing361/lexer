#include"prototype.hh"

using namespace std;

prototype::prototype( type ret, const std::string& name, std::vector<type>&& args )
  : mReturn( ret )
  , mName( name )
  , mArgs( args ){
}

string prototype::name() const{
  return mName;
}

