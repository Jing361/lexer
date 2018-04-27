#include"binary.hh"

using namespace std;

binary::binary( const string& Operator, expr_ptr lhs,
                                        expr_ptr rhs ):
  mOperator( Operator ),
  mLhs( move( lhs ) ),
  mRhs( move( rhs ) ){
}

string binary::generate() const{
  string instructions;

  instructions += mLhs->generate();
  instructions += mRhs->generate();

  if( mOperator == "+" ){
    instructions += "add\n";
  } else if( mOperator == "-" ){
    instructions += "sub\n";
  } else if( mOperator == "*" ){
    instructions += "mul\n";
  } else if( mOperator == "/" ){
    instructions += "div\n";
  }

  return instructions;
}

