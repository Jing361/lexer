#include<for.hh>

using namespace std;

for_loop::for_loop( stmnt_ptr initializer, expr_ptr cond, stmnt_ptr control, vector<expr_ptr>&& block )
  : mInitializer( move( initializer ) )
  , mConditional( move( cond ) )
  , mLoopControl( move( control ) )
  , mBlock( move( block ) ){
}

