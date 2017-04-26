#ifndef __FUNC_HH__
#define __FUNC_HH__

#include<iterator>
#include<memory>

#include"expression.hh"
#include"prototype.hh"

class func{
private:
  std::unique_ptr<prototype> mProto;
  std::vector<std::unique_ptr<expression> > mBody;

public:
  func( std::unique_ptr<prototype>&& proto, std::vector<std::unique_ptr<expression> >&& body ):
    mProto( std::move( proto ) ),
    mBody( std::move( body ) ){
  }
};

#endif

