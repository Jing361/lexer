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

public:
  ifExpr( std::unique_ptr<expression>&& cond,
          std::vector<std::unique_ptr<expression> >&& then,
          std::vector<std::unique_ptr<expression> >&& kElse ):
    mCond( std::move( cond ) ),
    mThen( std::move( then ) ),
    mElse( std::move( kElse ) ){
  }
};

#endif

