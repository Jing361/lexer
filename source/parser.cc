#include<iostream>
#include<string>

#include"parser.hh"

using namespace std;

int parser::getPrecedence(){
  try{
    return mOpPrecedence.at( mCurTok->lexeme );
  }catch( out_of_range ){
    return -1;
  }
}

unique_ptr<expression> parser::log_error( const string& str ){
  cout << "Log Error: " << str << endl;
  return nullptr;
}

expression
parser::parse_expression(){
}

void
parser::parse_toplevel(){
  while( mCurTok != mTokens.end() ){
    mTopLevel.push_back( parse_expression() );
    if( mCurTok->type != classification::SEMI ){
      throw runtime_error( "Expected semicolon" );
    } else {
      ++mCurTok;
    }
  }
}

/*unique_ptr<expression> parser::parse_bin_op( int lhsPrec, unique_ptr<expression> lhs ){
  while( true ){
    int curPrec = getPrecedence();

    if( curPrec < lhsPrec ){
      return lhs;
    }

    string op = mCurTok->tok;

    ++mCurTok;

    auto rhs = parse_primary();
    if( !rhs ){
      return nullptr;
    }

    int nextPrec = getPrecedence();
    if( nextPrec > curPrec ){
      rhs = parse_bin_op( curPrec + 1, move( rhs ) );
      if( !rhs ){
        return nullptr;
      }
    }

    lhs = make_unique<binary>( op, move( lhs ), move( rhs ) );
  }
}*/

