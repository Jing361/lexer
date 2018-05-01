#ifndef __WHILE_HH__
#define __WHILE_HH__

#include<vector>

class while_loop : public statement{
private:
  expr_ptr mCondition;
  std::vector<> mBlock;

public:
  while_loop( expr_ptr cond, std::vector<> block );
};

using while_ptr = unique_ptr<while_loop>;
using make_while = make_unique<while_loop>;

#endif

