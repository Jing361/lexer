#ifndef __BINARY_HH__
#define __BINARY_HH__

#include<memory>
#include<string>

#include"expression.hh"

class binary : public expression{
private:
  std::string mOperator;
  expr_ptr mLhs;
  expr_ptr mRhs;

public:
  binary( const std::string& Operator, expr_ptr lhs,
                                       expr_ptr rhs );

  std::string
  generate() const override;
};

#endif

