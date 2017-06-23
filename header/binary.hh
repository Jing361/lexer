#ifndef __BINARY_HH__
#define __BINARY_HH__

#include<memory>
#include<string>

#include"expression.hh"

class binary : public expression{
private:
  std::string mOperator;
  std::unique_ptr<expression> mLhs;
  std::unique_ptr<expression> mRhs;

public:
  binary( const std::string& Operator, std::unique_ptr<expression>&& lhs,
                                       std::unique_ptr<expression>&& rhs ):
    mOperator( Operator ),
    mLhs( std::move( lhs ) ),
    mRhs( std::move( rhs ) ){
  }

  std::string generate() override{
    return "";
  }
};

#endif

