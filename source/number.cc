#include<sstream>

#include<number.hh>

using namespace std;

number::number( double val )
  : base( "double" )
  , mValue( val ){
}

number::number( std::string val )
  : base( "double" ){
  stringstream ss( val );
  ss >> mValue;
}

string number::generate() const{
  return string( "push " ) + to_string( mValue ) + "\n";
}

