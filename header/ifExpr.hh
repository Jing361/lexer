#ifndef __IFEXPR_HH__
#define __IFEXPR_HH__

#include<memory>
#include<vector>

#include"expression.hh"

class ifExpr : public expression{
private:
  std::unique_ptr<expression> mCond;
  std::vector<std::unique_ptr<expression> > mThen;
  std::vector<std::unique_ptr<expression> > mElse;
  static unsigned int tmLabelCounter;

public:
  ifExpr( std::unique_ptr<expression>&& cond,
          std::vector<std::unique_ptr<expression> >&& then,
          std::vector<std::unique_ptr<expression> >&& kElse );

  virtual std::string generate() const override;
};

#endif

