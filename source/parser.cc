#include<sstream>
#include<iostream>
#include<string>

#include<parser.hh>

using namespace std;

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

expr_ptr
parser::parse_variable_declaration(){
  //extern?
  type t = parse_type();
  string s = parse_identifier();
  //'['integer']'
  //parse_initialization();

  return make_var( t, s );
}

stmnt_ptr
parser::parse_function_declaration(){
  //extern?
  
  type return_type = parse_type();
  identifier name = parse_identifier();

  if( mCurTok->type != classification::LPAREN ){
    throw runtime_error( "Expected '(' after function prototype name" );
  }

  ++mCurTok;

  vector<type> params = parse_param_list();

  if( mCurTok->type != classification::LPAREN ){
    throw runtime_error( "Expected ')' after function prototype parameter list" );
  }

  ++mCurTok;

  return make_proto( return_type, name, params );
}

type_ptr
parser::parse_type(){
  set<qualifer> quals;
  qualifer next;
  
  next = parse_qualifer();
  while( next != "" ){
    quals.insert( next );
    next = parse_qualifer();
  }

  return make_type( parse_typename(), quals );
  //pointers?
}

qualifier
parser::parse_qualifier(){
  qualifer ret;

  if( ret == "const"
   || ret == "volatile"
   || ret == "static" ){
    ret = mCurTok->lexeme;

    ++mCurTok;
  }

  return ret;
}

stmnt_ptr
parser::parse_typename(){
  return parse_identifier();
}

stmnt_ptr
parser::parse_type_spec(){
  parse_identifier();
  parse_type();
}

stmnt_ptr
parser::parse_identifier(){
  identifier ident = mCurTok->lexeme;
  char first = ident[0];

  if( first >= '0' && first <= '9' ){
    throw runtime_error( "Expected non_digit character to start identifier" );
  }

  return ident;
}

stmnt_ptr
parser::parse_initialization(){
/*  '='
  parse_value();*/
}

vector<type> params;
parser::parse_param_list(){
  vector<type> params;

  type next = parse_param();
  if( next.type_name() != "" ){
    params.emplace_back( next );
  }

  // this can be implemented with recursion by using an if, and a recursive call
  // chose not to implement recursively for performance
  while( mCurTok->type == classification::COMMA ){
    ++mCurTok;

    next = parse_param();

    if( next.type_name() == "" ){
      throw runtime_error( "Error parsing parameter" );
    }
  }

  return params;
}

type
parser::parse_param(){
  type t = parse_type();

  //identifier is parsed for syntax
  parse_identifier();

  return t;
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
  if( mCurTok->type == classification::IDENTIFIER ){
    return parse_variable_declaration();
  } else if( mCurTok->type == classification::IF ){
    return parse_branch();
  } else if( mCurTok->type == classification::FOR ){
    return parse_for();
  } else if( mCurTok->type == classification::WHILE ){
    return parse_while();
  } else if( mCurTok->type == classification::TYPEDEF ){
    return parse_type_spec();
  } else {
    throw runtime_error( "Could not parse statement" );
  }
}

vector<expr_ptr>
parser::parse_arg_list(){
  vector<expr_ptr> args;

  type next = parse_arg();
  if( next.type_name() != "" ){
    args.emplace_back( next );
  }

  // this can be implemented with recursion by using an if, and a recursive call
  // chose not to implement recursively for performance
  while( mCurTok->type == classification::COMMA ){
    ++mCurTok;

    next = parse_arg();

    if( next.type_name() == "" ){
      throw runtime_error( "Error parsing argument" );
    }
  }

  return args;
}

expr_ptr
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
  if( mCurTok->type != classification::FOR ){
    throw runtime_error( "Expected 'for'" );
  }

  ++mCurTok;

  if( mCurTok->type != classification::LPAREN ){
    throw runtime_error( "Expected '(' after 'for'" );
  }

  ++mCurtok;

  stmnt_ptr init = parse_statement();
  if( mCurTok->type != classification::SEMI ){
    throw runtime_error( "Expected ';' after for loop initialization" );
  }

  ++mCurTok;

  expr_ptr cond = parse_expression();
  if( mCurTok->type != classification::SEMI ){
    throw runtime_error( "Expected ';' after for loop conditional" );
  }

  ++mCurTok;

  stmnt_ptr cntrl = parse_statement();
  if( mCurTok->type != classification::SEMI ){
    throw runtime_error( "Expected ';' after for loop control" );
  }

  ++mCurTok;

  if( (++mCurTok)->type != classification::RPAREN ){
    throw runtime_error( "Expected ')' after for loop parameters" );
  }

  ++mCurtok;

  return make_for( init, cond, cntrl, parse_brackets() );
}

while_ptr
parser::parse_while(){
  if( mCurTok->type != classification::WHILE ){
    throw runtime_error( "Expected 'while'" );
  }

  ++mCurTok;

  expr_ptr condition = parse_expression();

  return make_while( condition, parse_brackets() );
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
    exprs.emplace_back( parse_stuff() );
  } while( bracket && mCurTok->type != classification::RBRACKET );

  return exprs;
}


//! @todo expression parsing needs fixing
expr_ptr
parser::parse_expression(){
  bool paren;

  if( mCurTok->type == classification::LPAREN ){
    ++mCurTok;
    paren = true;
  }

  auto E = parse_binary( 0, parse_primary() );

  if( paren && mCurTok->type != classification::RPAREN ){
    throw runtime_error( "Expected ')'" );
  }

  return E;
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

    auto rhs = expression();

    int next_precedence = getPrecedence();

    if( next_precedence > op_precedence ){
      rhs = parse_binary( op_precedence + 1, move( rhs ) );
    }

    lhs = make_unique<binary>( op, move( lhs ), move( rhs ) );
  }
}

expr_ptr
parser::parse_stuff(){
  switch( mCurTok->type ){
  case classification::IF:
  case classification::FOR:
  case classification::WHILE:
  case classification::TYPEDEF:
    return parse_statement();
    break;

  default:
    auto next_tok = mCurTok;
    ++next_tok;

    if( next_tok->type == classification::IDENTIFIER ){
      return parse_statement();
    } else {
      return parse_expression();
    }
    break;
  }
}

