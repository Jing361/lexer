#ifndef __IFEXPR_HH__
#define __IFEXPR_HH__

#include<memory>
#include<vector>

#include"expression.hh"

// if expressions are more statements, but this is fine
class ifExpr : public expression{
private:
  expr_ptr mCond;
  std::vector<expr_ptr> mThen;
  std::vector<expr_ptr> mElse;
  static unsigned int tmLabelCounter;

public:
  ifExpr( expr_ptr cond,
          std::vector<expr_ptr> then,
          std::vector<expr_ptr> kElse = std::vector<expr_ptr>() );

  virtual
  std::string
  generate() const override;
};

#endif

