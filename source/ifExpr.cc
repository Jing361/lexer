#include"ifExpr.hh"

using namespace std;

unsigned int ifExpr::tmLabelCounter;

ifExpr::ifExpr( expression cond,
                vector<expression> then,
                vector<expression> kElse ):
  mCond( move( cond ) ),
  mThen( move( then ) ),
  mElse( move( kElse ) ){
}

string ifExpr::generate() const{
  string instructions;
  string then_label( "then_" + to_string( tmLabelCounter++ ) );
  string end_label( "if_end_" + to_string( tmLabelCounter++ ) );
  string then_text;
  string else_text;

  for( unsigned int i = 0; i < mThen.size(); ++i ){
    then_text += mThen[i].generate();
  }

  for( unsigned int i = 0; i < mElse.size(); ++i ){
    else_text += mElse[i].generate();
  }

  //condition check
  instructions += mCond.generate();
  instructions += "bz " + then_label + "\n";

  //else execution
  instructions += else_text;
  instructions += "goto " + end_label + "\n";

  //then segment
  instructions += then_label + ":\n";
  instructions += then_text;

  //end marker
  instructions += end_label + ":\n";

  return instructions;
}

