#ifndef __FOR_HH__
#define __FOR_HH__

#include<vector>

#include<statement.hh>
#include<expression.hh>

class for_loop : public statement{
private:
  stmnt_ptr mInitializer;
  expr_ptr mConditional;
  stmnt_ptr mLoopControl;
  // statements and expressions?
  // type erasure?(both will use some kind of code generation?)
  std::vector<expr_ptr> mBlock;

public:
  for_loop( stmnt_ptr initializer, expr_ptr cond, stmnt_ptr control,
            std::vector<expr_ptr>&& block );
};

using for_ptr = std::unique_ptr<for_loop>;
//extern const decltype( std::make_unique<for_loop> )& make_for;

#endif

