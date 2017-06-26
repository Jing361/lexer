#include"number.hh"

using namespace std;

number::number( double val ):
  mValue( val ){
}

string number::generate() const{
  return string( "push " ) + to_string( mValue ) + "\n";
}

