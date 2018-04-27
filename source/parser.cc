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
    return parse_identifier();
  break;

  case classification::IF:
    return parse_branch();
  break;

  default:
    throw runtime_error( "Expected primary expression" );
  break;
  }
}

expr_ptr
parser::parse_paren(){
  parse_expression();

  if( mCurTok->type != classification::RPAREN ){
    throw runtime_error( "Expected ')'" );
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

    int next_precedence = getPrecedence();

    if( next_precedence > op_precedence ){
      rhs = parse_binary( op_precedence + 1, move( rhs ) );
    }

    lhs = make_unique<binary>( op, move( lhs ), move( rhs ) );
  }
}

expr_ptr
parser::parse_identifier(){
  string ident = mCurTok++->lexeme;

  if( mCurTok->type == classification::LPAREN ){
    // function call
    ++mCurTok;
    vector<expr_ptr> args;
    bool args_done = false;

    //get args
    while( mCurTok->type != classification::RPAREN && !args_done ){
      args.push_back( parse_expression() );

      if( mCurTok->type == classification::RPAREN ){
        args_done = true;
      } else if( mCurTok->type == classification::COMMA ){
      } else {
        throw runtime_error( "Expected either ',' or ')' for function call" );
      }

      ++mCurTok;
    }

    ++mCurTok;
  } else if( mCurTok->type == classification::IDENTIFIER ){
    // Declaration
  } else {
    // just variable usage
    return make_unique<variable>( ident );
  }
}

expr_ptr
parser::parse_branch(){
  if( ++mCurTok++->type != classification::LPAREN ){
    throw runtime_error( "Expected '(' after 'if'" );
  }

  auto condition = parse_expression();
  auto true_br = parse_expression();

  if( mCurtok->type == classification::ELSE ){
    return make_unique<ifExpr>( condition, true_br, parse_expression() );
  } else {
    return make_unique<ifExpr>( condition, true_br );
  }
}
void
parser::parse_toplevel(){
  while( mCurTok != mTokens.end() ){
    mTopLevel.push_back( parse_expression() );

    if( mCurTok->type != classification::SEMI ){
      throw runtime_error( "Expected semicolon after expression" );
    } else {
      ++mCurTok;
    }
  }
}

