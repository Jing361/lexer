#ifndef __NUMBER_HH__
#define __NUMBER_HH__

#include<string>
#include<sstream>

#include"expression.hh"

class number : public expression{
private:
  double mValue;

public:
  number( double val ):
    mValue( val ){
  }

  virtual std::string generate() override{
    return std::string( "push " ) + std::to_string( mValue ) + "\n";
  }
};

#endif

