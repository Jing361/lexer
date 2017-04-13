#ifndef __FUNCTION_HH__
#define __FUNCTION_HH__

#include<iterator>
#include<memory>

#include"expression.hh"
#include"prototype.hh"

class function{
private:
  std::unique_ptr<prototype> mProto;
  std::unique_ptr<expression> mBody;

public:
  function( std::unique_ptr<prototype>&& proto, std::unique_ptr<expression>&& body ):
    mProto( std::move( proto ) ),
    mBody( std::move( body ) ){
  }
};

#endif

