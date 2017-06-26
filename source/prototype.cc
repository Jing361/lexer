#include"prototype.hh"

using namespace std;

prototype::prototype( const string& name, vector<string>&& args ):
  mName( name ),
  mArgs( args ){
}

string prototype::name() const{
  return mName;
}

