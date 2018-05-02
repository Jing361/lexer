#include<while.hh>

while_loop::while_loop( expr_ptr cond, std::vector<expr_ptr>&& block )
  : mCondition( move( cond ) )
  , mBlock( move( block ) ){
}

