#ifndef __IFEXPR_HH__
#define __IFEXPR_HH__

#include<memory>
#include<vector>

#include"expression.hh"

// if expressions are more statements, but this is fine
class ifExpr : public expression{
private:
  expression mCond;
  std::vector<expression> mThen;
  std::vector<expression> mElse;
  static unsigned int tmLabelCounter;

public:
  ifExpr( expression cond,
          std::vector<expression> then,
          std::vector<expression> kElse = nullptr );

  virtual
  std::string
  generate() const override;
};

#endif

