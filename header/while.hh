#ifndef __WHILE_HH__
#define __WHILE_HH__

#include<vector>

#include<expression.hh>
#include<statement.hh>

class while_loop : public statement{
private:
  expr_ptr mCondition;
  std::vector<expr_ptr> mBlock;

public:
  while_loop( expr_ptr cond, std::vector<expr_ptr>&& block );

  std::string
  generate() const {}
};

using while_ptr = std::unique_ptr<while_loop>;
//extern const decltype( std::make_unique<while_loop> )& make_while;

#endif

