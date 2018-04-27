#include<iostream>
#include<string>

#include"parser.hh"

using namespace std;

int
parser::getPrecedence(){
  try{
    return mOpPrecedence.at( mCurTok->lexeme );
  }catch( out_of_range ){
    return -1;
  }
}

expr_ptr
parser::log_error( const string& str ){
  cout << "Log Error: " << str << endl;
  return nullptr;
}

expr_ptr
parser::parse_expression(){
  return parse_binary( 0, parse_primary() );
}

// primary expressions include literals ( "string literal", 'C', 42, 6.9,
// true, etc ) and identifiers ( x in x + 2 )
expr_ptr
parser::parse_primary(){
  switch( mCurTok->type ){
  case classification::LPAREN:
    return parse_paren();
  break;

  case classification::NUMBER:
    return make_unique<number>( mCurTok->lexeme );
  break;

  case classification::IDENTIFIER:
  break;

  case classification::IF:
  break;

  default:
    throw error;
  break;
  }
}

expr_ptr
parser::parse_paren(){
  parse_expression();

  if( mCurTok->type != classification::RPAREN ){
    throw error //expected ')'
  }
}

expr_ptr
parser::parse_binary( int lhs_precedence, expr_ptr lhs ){
  while( true ){
    int op_precedence = getPrecedence();

    if( op_precedence < lhs_precedence ){
      return lhs;
    }

    string op = mCurTok->tok;

    ++mCurTok;

    auto rhs = parse_primary();
    if( !rhs ){
      throw error;
    }

    int next_precedence = getPrecedence();

    if( next_precedence > op_precedence ){
      rhs = parse_binary( op_precedence + 1, move( rhs ) );
      if( !rhs ){
        throw error;
      }
    }

    lhs = make_unique<binary>( op, move( lhs ), move( rhs ) );
  }
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

