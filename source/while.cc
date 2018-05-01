#include<while.hh>

while_loop::while_loop( expr_ptr cond, std::vector<> block )
  : mCondition( cond )
  , mBlock( block ){
}

