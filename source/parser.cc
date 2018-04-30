#include<sstream>
#include<iostream>
#include<string>

#include"parser.hh"

using namespace std;

int
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
  ++mCurTok;

  auto E = parse_expression();

  if( mCurTok->type != classification::RPAREN ){
    throw runtime_error( "Expected ')'" );
  }

  return E;
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

    return make_unique<call>( ident, move( args ) );
  } else if( mCurTok->type == classification::IDENTIFIER ){
    // Declaration
    return make_unique<variable>( ident );
  } else {
    // just variable usage
    return make_unique<variable>( ident );
  }
}

void
parser::parse_toplevel(){
  while( mCurTok != mTokens.end() ){
    mTopLevel.push_back( parse_expression() );

    if( mCurTok->type != classification::SEMI ){
      string row;
      string column;

      stringstream ss;
      ss << mCurTok->location.first;
      ss >> row;

      throw runtime_error( string( "Expected semicolon after expression " ) + row );
    } else {
      ++mCurTok;
    }
  }
}

int
parser::getPrecedence(){
  try{
    return mOpPrecedence.at( mCurTok->lexeme );
  }catch( out_of_range ){
    return -1;
  }
}

stmnt_ptr
parser::parse_prog_statement(){
  parse_statement();
  xor
  parse_function_declaration();
}

stmnt_ptr
parser::parse_variable_declaration(){
  extern
  parse_type();
  parse_identifier();
  '['integer']'
  parse_initialization();
}

stmnt_ptr
parser::parse_function_declaration(){
  extern
  parse_type();
  parse_identifier();
  '('
  parse_param_list();
  ')'
}

stmnt_ptr
parser::parse_type(){
  parse_qualifiers();*
  parse_typename();
  '*'
}

stmnt_ptr
parser::parse_qualifiers(){
  const
  volatile
  static
}

stmnt_ptr
parser::parse_typename(){
  parse_identifier();
}

stmnt_ptr
parser::parse_type_spec(){
  typedef
  parse_identifier();
  parse_type();
}

stmnt_ptr
parser::parse_identifier(){
  parse_non_digit_ident_char();
  parse_ident_char();*
}

stmnt_ptr
parser::parse_initialization(){
  '='
  parse_value();
}

stmnt_ptr
parser::parse_param_list(){
  parse_param();
  if( ',' )
  parse_param_list();
}

stmnt_ptr
parser::parse_param(){
  parse_type();
  parse_identifier();
}

stmnt_ptr
parser::parse_non_digit_ident_char(){
  [a-zA-Z_]
}

stmnt_ptr
parser::parse_ident_char(){
  parse_non_digit_ident_char();
  or
  parse_digit();
}

stmnt_ptr
parser::parse_integer(){
  parse_digit();*
}

stmnt_ptr
parser::parse_number(){
  parse_integer();
  if( '.' )
  parse_integer();
}

expr_ptr
parser::parse_value(){
  parse_expression();
}

stmnt_ptr
parser::parse_statement(){
  parse_variable_declaration();
  xor
  parse_branch();
  xor
  parse_for();
  xor
  parse_while();
  xor
  parse_type_spec();
}

stmnt_ptr
parser::parse_arg_list(){
  parse_arg();
  if( ',' )
  parse_arg_list();
}

stmnt_ptr
parser::parse_arg(){
  parse_expression();
}

stmnt_ptr
parser::parse_function_definition(){
  parse_function_declaration();
  parse_brackets();
}

stmnt_ptr
parser::parse_branch(){
  if( (++mCurTok)->type != classification::LPAREN ){
    throw runtime_error( "Expected '(' after 'if'" );
  }
  ++mCurTok;

  auto condition = parse_expression();
  if( mCurTok++->type != classification::RPAREN ){
    throw runtime_error( "Expected ')' after conditional" );
  }
  auto true_br = parse_brackets();

  if( mCurTok->type == classification::ELSE ){
    return make_unique<ifExpr>( move( condition ), move( true_br ), parse_brackets() );
  } else {
    return make_unique<ifExpr>( move( condition ), move( true_br ) );
  }
}

stmnt_ptr
parser::parse_for(){
  if( (++mCurTok)->type != classification::LPAREN ){
    throw runtime_error( "Expected '(' after 'for'" );
  }
  ++mCurtok;

  parse_statement();
  if( not ';' )
  throw;
  else
  ++mCurTok; ';'

  parse_expression();
  if( not ';' )
  throw;
  else
  ++mCurTok; ';'

  parse_statement();
  if( not ';' )
  throw;
  else
  ++mCurTok; ';'

  parse_brackets();
}

stmnt_ptr
parser::parse_while(){
  'while'
  parse_expression();
  parse_brackets();
}

std::vector<expr_ptr>
parser::parse_brackets(){
  bool bracket;

  if( mCurTok->type == classification::LBRACKET ){
    ++mCurTok;
    bracket = true;
  }

  vector<expr_ptr> exprs;

  do{
    exprs.emplace_back( parse_expression() );
  } while( bracket && mCurTok->type != classification::RBRACKET );

  return exprs;
}


expr_ptr
parser::parse_expression(){
}

expr_ptr
parser::parse_binary( int lhs_precedence, expr_ptr lhs ){
  while( true ){
    int op_precedence = getPrecedence();

    if( op_precedence < lhs_precedence ){
      return lhs;
    }

    string op = mCurTok->lexeme;

    ++mCurTok;

    auto rhs = parse_primary();

    int next_precedence = getPrecedence();

    if( next_precedence > op_precedence ){
      rhs = parse_binary( op_precedence + 1, move( rhs ) );
    }

    lhs = make_unique<binary>( op, move( lhs ), move( rhs ) );
  }
}

