#include"call.hh"

using namespace std;

call::call( const string& callee, vector<unique_ptr<expression> >&& args ):
  mCallee( callee ),
  mArgs( move( args ) ){
}

string call::generate() const{
  string instructions;

  for( unsigned int i = 0; i < mArgs.size(); ++i ){
    instructions += mArgs[i]->generate();
  }

  return instructions + "call " + mCallee + "\n";
}

