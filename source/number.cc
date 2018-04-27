#include<sstream>

#include"number.hh"

using namespace std;

number::number( double val ):
  mValue( val ){
}

number::number( std::string val ){
  stringstream ss( val );
  ss >> mValue
}

string number::generate() const{
  return string( "push " ) + to_string( mValue ) + "\n";
}

