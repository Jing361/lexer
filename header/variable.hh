#ifndef __VARIABLE_HH__
#define __VARIABLE_HH__

#include<string>

#include"expression.hh"

class variable : public expression{
private:
  std::string mName;

public:
  variable( const std::string& name ):
    mName( name ){
  }
};

#endif

