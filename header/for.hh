#ifndef __FOR_HH__
#define __FOR_HH__

#include<statement.hh>
#include<expression.hh>

class for_loop : public statement{
private:
  stmnt_ptr mInitializer;
  expr_ptr mConditional;
  stmnt_ptr mLoopControl;
  // statements and expressions?
  // type erasure?(both will use some kind of code generation?)
  std::vector<> mBlock;

public:
  for_loop( stmnt_ptr initializer, expr_ptr cond, stmnt_ptr control,
            const std::vector<> block );
};

using for_ptr = unique_ptr<for_loop>;
using make_for = make_unique<for_loop>;

#endif

