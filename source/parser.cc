#include<iostream>

#include"parser.hh"

using namespace std;

int parser::getPrecedence(){
  try{
    return mOpPrecedence.at( mCurTok->second );
  }catch( out_of_range ){
    return -1;
  }
}

unique_ptr<expression> parser::log_error( const string& str ){
  cout << "Log Error: " << str << endl;
  return nullptr;
}

unique_ptr<prototype> parser::log_proto_error( const string& str ){
  cout << "Log Error: " << str << endl;
  return nullptr;
}

unique_ptr<number> parser::parse_number(){
  return make_unique<number>( stod( mCurTok++->second ) );
}

unique_ptr<expression> parser::parse_paren(){
  ++mCurTok;
  auto E = parse_expression();

  if( !E ){
    return nullptr;
  }

  if( mCurTok->second != ")" ){
    return log_error( "expected ')'" );
  }

  ++mCurTok;
  return E;
}

unique_ptr<expression> parser::parse_ident(){
  string name = mCurTok->second;
  ++mCurTok;

  if( mCurTok->second != "(" ){
    return make_unique<variable>( name );
  }
  ++mCurTok;

  vector<unique_ptr<expression> > args;
  bool bEndArgs = false;

  while( mCurTok->second != ")" && !bEndArgs ){
    if( auto arg = parse_expression() ){
      args.push_back( move( arg ) );
    } else {
      return nullptr;
    }

    if( mCurTok->second == ")" ){
      bEndArgs = true;
    } else if( mCurTok->second != "," ){
      return log_error( "Expected ')' or ',' in argument list; found: '" + mCurTok->second + "'." );
    }

    ++mCurTok;
  }

  return make_unique<call>( name, move( args ) );
}

unique_ptr<expression> parser::parse_expression(){
  auto lhs = parse_primary();

  if( !lhs ){
    return nullptr;
  }

  return parse_bin_op( 0, move( lhs ) );
}

std::vector<std::unique_ptr<expression> > parser::parse_brackets(){
  std::vector<std::unique_ptr<expression> > body;
  bool bBrackets = false;

  if( mCurTok->second == "{" ){
    bBrackets = true;
    ++mCurTok;
  }

  do{
    if( auto E = parse_expression() ){
      body.emplace_back( move( E ) );
    }

    // skips ';'
    ++mCurTok;
  }while( bBrackets && mCurTok->second != "}" );

  if( mCurTok->second == "}" ){
    ++mCurTok;
  }

  return body;
}

unique_ptr<expression> parser::parse_bin_op( int lhsPrec, unique_ptr<expression> lhs ){
  while( true ){
    int curPrec = getPrecedence();

    if( curPrec < lhsPrec ){
      return lhs;
    }

    string op = mCurTok->second;

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
}

unique_ptr<prototype> parser::parse_proto(){
  if( mCurTok->first != CLASS_IDENT ){
    return log_proto_error( "Expected function name in prototype." );
  }

  string fnName = mCurTok->second;
  ++mCurTok;

  if( mCurTok->second != "(" ){
    return log_proto_error( "Expected '(' in prototype." );
  }

  vector<string> args;
  while( ( ++mCurTok )->first == CLASS_IDENT ){
    args.push_back( mCurTok->second );
  }

  if( mCurTok->second != ")" ){
    return log_proto_error( "Expected ')' in prototype." );
  }

  ++mCurTok;

  return make_unique<prototype>( fnName, move( args ) );
}

unique_ptr<func> parser::parse_def(){
  ++mCurTok;

  auto proto = parse_proto();

  if( !proto ){
    return nullptr;
  }

  std::vector<std::unique_ptr<expression> > body = parse_brackets();

  if( !body.empty() && body.front() ){
    return make_unique<func>( move( proto ), move( body ) );
  } else {
    return nullptr;
  }
}

unique_ptr<prototype> parser::parse_extern(){
  ++mCurTok;

  return parse_proto();
}

unique_ptr<func> parser::parse_top(){
  if( auto E = parse_expression() ){
    vector<unique_ptr<expression> > vec;
    auto proto = make_unique<prototype>( "", vector<string>() );

    vec.emplace_back( move( E ) );

    return make_unique<func>( move( proto ), move( vec ) );
  }

  return nullptr;
}

unique_ptr<ifExpr> parser::parse_if(){
  ++mCurTok;

  auto P = parse_paren();
  auto T = parse_brackets();
  vector<unique_ptr<expression> > E;

  if( mCurTok->first == CLASS_ELSE ){
    ++mCurTok;

    E = parse_brackets();
  }

  return make_unique<ifExpr>( move( P ), move( T ), move( E ) );
}

unique_ptr<expression> parser::parse_primary(){
  if( mCurTok == mTokens.end() ){
    return log_error( "Reached early end of token stream." );
  }

  switch( mCurTok->first ){
  case CLASS_PAREN:
    return parse_paren();
  break;

  case CLASS_IDENT:
    return parse_ident();
  break;

  case CLASS_INTEGER:
    return parse_number();
  break;

  case CLASS_IF:
    return parse_if();
  break;

  case CLASS_EOF:
    return log_error( "Unexpected end of file." );
  break;

  default:
    return log_error( "Unknown token: '" + mCurTok->second + "' when expecting an expression." );
  break;
  }
}

void parser::handle_def(){
  if( parse_def() ){
    cout << "Parsed function def.\n";
  } else {
    ++mCurTok;
  }
}

void parser::handle_extern(){
  if( parse_extern() ){
    cout << "Parsed extern.\n";
  } else {
    ++mCurTok;
  }
}

void parser::handle_top(){
  if( parse_top() ){
    cout << "Parsed top.\n";
  } else {
    ++mCurTok;
  }
}

void parser::main_loop(){
  while( true ){
    switch( mCurTok->first ){
    case CLASS_EOF:
      return;
    break;

    case CLASS_SEMI:
      ++mCurTok;
    break;

    case CLASS_DEF:
      handle_def();
    break;

    case CLASS_EXTERN:
      handle_extern();
    break;

    default:
      handle_top();
    break;
    }
  }
}

