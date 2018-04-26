#ifndef __BINARY_HH__
#define __BINARY_HH__

#include<memory>
#include<string>

#include"expression.hh"

class binary : public expression{
private:
  std::string mOperator;
  expression mLhs;
  expression mRhs;

public:
  binary( const std::string& Operator, expression lhs,
                                       expression rhs );

  std::string
  generate() const override;
};

#endif

