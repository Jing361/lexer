#include"function.hh"

using namespace std;

func::func( unique_ptr<prototype>&& proto, vector<unique_ptr<expression> >&& body ):
  mProto( move( proto ) ),
  mBody( move( body ) ){
}

string func::name() const{
  return mProto->name();
}

string func::generate() const{
  string instructions( mProto->name() + ":\n" );

  for( unsigned int i = 0; i < mBody.size(); ++i ){
    instructions += mBody[i]->generate();
  }

  return instructions + "return\n";
}

