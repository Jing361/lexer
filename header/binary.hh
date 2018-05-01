#ifndef __BINARY_HH__
#define __BINARY_HH__

#include<memory>
#include<string>

#include<expression.hh>

class binary : public expression{
private:
  using base = expression;

  std::string mOperator;
  expr_ptr mLhs;
  expr_ptr mRhs;

public:
  binary( const std::string& Operator, expr_ptr lhs,
                                       expr_ptr rhs );

  std::string
  generate() const override;
};

using bin_ptr = std::unique_ptr<binary>;
//extern const decltype( std::make_unique<binary> )& make_bin;

#endif

