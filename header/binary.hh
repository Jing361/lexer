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
    std::string instructions;

    instructions += mLhs->generate();
    instructions += mRhs->generate();

    if( mOperator == "+" ){
      instructions += "add\n";
    } else if( mOperator == "-" ){
      instructions += "sub\n";
    } else if( mOperator == "*" ){
      instructions += "mul\n";
    } else if( mOperator == "/" ){
      instructions += "div\n";
    }

    return instructions;
  }
};

#endif

