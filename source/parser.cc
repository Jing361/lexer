#include<sstream>
#include<iostream>
#include<string>

#include<parser.hh>
#include<for.hh>

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
    //! @todo retrieve actual typename
    return make_unique<variable>( type( "void" ), parse_identifier() );
  break;

  default:
    throw runtime_error( string( "Expected primary expression:" ) + to_string( mCurTok->location.first ) + "\n\tFound: " + mCurTok->lexeme );
  break;
  }
}

expr_ptr
parser::parse_paren(){
  ++mCurTok;

  auto E = parse_expression();

  if( mCurTok->type != classification::RPAREN ){
    throw runtime_error( string( "Expected ')':" ) + to_string( mCurTok->location.first ) + "\n\tFound: " + mCurTok->lexeme );
  }

  return E;
}

void
parser::parse_toplevel(){
  while( mCurTok != mTokens.end() ){
    mTopLevel.push_back( parse_prog_statement() );

    if( mCurTok->type != classification::SEMI ){
      throw runtime_error( string( "Expected semicolon after expression " ) + to_string( mCurTok->location.first ) + "\n\tFound: " + mCurTok->lexeme );
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
  if( mCurTok->type == classification::TYPEDEF 
   || mCurTok->type == classification::EXTERN ){
    return parse_decl_stmnt();
  } else if( mCurTok->type == classification::IDENTIFIER ){
    auto next = mCurTok;
    ++next;

    if( next->type == classification::IDENTIFIER ){
      ++next;

      if( next->type == classification::LPAREN ){
        return parse_function_declaration();
      } else {
        return parse_decl_stmnt();
      }
    } else {
      throw runtime_error( string( "Expected identifier:" ) + to_string( mCurTok->location.first ) + "\n\tFound: " + mCurTok->lexeme  );
    }
  }
}

stmnt_ptr
parser::parse_decl_stmnt(){
  if( mCurTok->type == classification::TYPEDEF ){
    return parse_type_spec();
  } else {
    return parse_variable_declaration();
  }
}

//! @todo add array and initialization parsing
stmnt_ptr
parser::parse_variable_declaration(){
  //extern?
  type t = parse_type();
  identifier s = parse_identifier();
  //'['integer']'
  //parse_initialization();

  return make_unique<variable>( t, s );
}

stmnt_ptr
parser::parse_function_declaration(){
  //extern?
  
  type return_type = parse_type();
  identifier name = parse_identifier();

  if( mCurTok->type != classification::LPAREN ){
    throw runtime_error( string( "Expected '(' after function prototype name:" ) + to_string( mCurTok->location.first ) + "\n\tFound: " + mCurTok->lexeme );
  }

  ++mCurTok;

  vector<type> params = parse_param_list();

  if( mCurTok->type != classification::LPAREN ){
    throw runtime_error( string( "Expected ')' after function prototype parameter list:" ) + to_string( mCurTok->location.first ) + "\n\tFound: " + mCurTok->lexeme );
  }

  ++mCurTok;

  return make_unique<prototype>( return_type, name, move( params ) );
}

type
parser::parse_type(){
  set<qualifier> quals;
  qualifier next;
  
  next = parse_qualifier();
  while( next != "" ){
    quals.insert( next );
    next = parse_qualifier();
  }

  return type( parse_typename(), quals );
  //! @todo parse pointer types?
}

qualifier
parser::parse_qualifier(){
  qualifier ret;

  if( ret == "const"
   || ret == "volatile"
   || ret == "static" ){
    ret = mCurTok->lexeme;

    ++mCurTok;
  }

  return ret;
}

parser::identifier
parser::parse_typename(){
  return parse_identifier();
}

stmnt_ptr
parser::parse_type_spec(){
  parse_identifier();
  parse_type();
}

parser::identifier
parser::parse_identifier(){
  string ident = mCurTok->lexeme;
  char first = ident[0];

  ++mCurTok;

  if( first >= '0' && first <= '9' ){
    throw runtime_error( string( "Expected non_digit character to start identifier:" ) + to_string( mCurTok->location.first ) + "\n\tFound: " + mCurTok->lexeme );
  }

  return ident;
}

stmnt_ptr
parser::parse_initialization(){
/*  '='
  parse_value();*/
}

vector<type>
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
      throw runtime_error( string( "Error parsing parameter:" ) + to_string( mCurTok->location.first ) + "\n\tFound: " + mCurTok->lexeme );
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
  //parse_digit();*
}

stmnt_ptr
parser::parse_number(){
  /*parse_integer();
  if( '.' )
  parse_integer();*/
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
    throw runtime_error( string( "Could not parse statement:" ) + to_string( mCurTok->location.first ) );
  }
}

vector<expr_ptr>
parser::parse_arg_list(){
  if( mCurTok->type != classification::LPAREN ){
    throw runtime_error( string( "Expected '(' for a function call" ) + to_string( mCurTok->location.first ) + "\n\tFound: " + mCurTok->lexeme );
  }

  ++mCurTok;

  vector<expr_ptr> args;

  // this can be implemented with recursion by using an if, and a recursive call
  // chose not to implement recursively for performance
  while( mCurTok->type != classification::RPAREN ){
    args.emplace_back( move( parse_arg() ) );

    if( mCurTok->type != classification::COMMA ){
      break;
    }
  }

  if( mCurTok->type != classification::RPAREN ){
    throw runtime_error( string( "Expected ')' at the end of function call:" ) + to_string( mCurTok->location.first ) + "\n\tFound: " + mCurTok->lexeme );
  }

  return args;
}

expr_ptr
parser::parse_arg(){
  return parse_expression();
}

stmnt_ptr
parser::parse_function_definition(){
  parse_function_declaration();
  parse_brackets();
}

stmnt_ptr
parser::parse_branch(){
  if( (++mCurTok)->type != classification::LPAREN ){
    throw runtime_error( string( "Expected '(' after 'if':" ) + to_string( mCurTok->location.first ) + "\n\tFound: " + mCurTok->lexeme );
  }
  ++mCurTok;

  auto condition = parse_expression();
  if( mCurTok++->type != classification::RPAREN ){
    throw runtime_error( string( "Expected ')' after conditional:" ) + to_string( mCurTok->location.first ) + "\n\tFound: " + mCurTok->lexeme );
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
    throw runtime_error( string( "Expected 'for':" ) + to_string( mCurTok->location.first ) + "\n\tFound: " + mCurTok->lexeme );
  }

  ++mCurTok;

  if( mCurTok->type != classification::LPAREN ){
    throw runtime_error( string( "Expected '(' after 'for':" ) + to_string( mCurTok->location.first ) + "\n\tFound: " + mCurTok->lexeme );
  }

  ++mCurTok;

  stmnt_ptr init = parse_statement();
  if( mCurTok->type != classification::SEMI ){
    throw runtime_error( string( "Expected ';' after for loop initialization:" ) + to_string( mCurTok->location.first ) + "\n\tFound: " + mCurTok->lexeme );
  }

  ++mCurTok;

  expr_ptr cond = parse_expression();
  if( mCurTok->type != classification::SEMI ){
    throw runtime_error( string( "Expected ';' after for loop conditional:" ) + to_string( mCurTok->location.first ) + "\n\tFound: " + mCurTok->lexeme );
  }

  ++mCurTok;

  stmnt_ptr cntrl = parse_statement();
  if( mCurTok->type != classification::SEMI ){
    throw runtime_error( string( "Expected ';' after for loop control:" ) + to_string( mCurTok->location.first ) + "\n\tFound: " + mCurTok->lexeme );
  }

  ++mCurTok;

  if( (++mCurTok)->type != classification::RPAREN ){
    throw runtime_error( string( "Expected ')' after for loop parameters:" ) + to_string( mCurTok->location.first ) + "\n\tFound: " + mCurTok->lexeme );
  }

  ++mCurTok;

  return make_unique<for_loop>( move( init ), move( cond ), move( cntrl ), move( parse_brackets() ) );
}

while_ptr
parser::parse_while(){
  if( mCurTok->type != classification::WHILE ){
    throw runtime_error( string( "Expected 'while':" ) + to_string( mCurTok->location.first ) + "\n\tFound: " + mCurTok->lexeme );
  }

  ++mCurTok;

  expr_ptr condition = parse_expression();

  return make_unique<while_loop>( move( condition ), move( parse_brackets() ) );
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
//! @todo doesn't parse function calls
  return parse_binary( 0, parse_primary() );
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

    auto rhs = parse_expression();

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

