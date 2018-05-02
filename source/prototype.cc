#include<prototype.hh>

using namespace std;

prototype::prototype( type ret, const string& name, vector<type>&& args )
  : mReturn( ret )
  , mName( name )
  , mArgs( args ){
}

string
prototype::name() const{
  return mName;
}

string
prototype::generate() const{
}

