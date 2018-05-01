#include<for.hh>

for_loop::for_loop( stmnt_ptr initializer, expr_ptr cond, stmnt_ptr control, const std::vector<> block )
  : mInitializer( initializer )
  , mConditional( cond )
  , mLoopControl( control )
  , mBlock( block ){
}

