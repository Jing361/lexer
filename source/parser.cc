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

std::unique_ptr<number> parser::parse_number(){
  double value;
  std::stringstream ss( mCurTok->second );

  ss >> value;

  ++mCurTok;

  return std::make_unique<number>( value );
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

  while( mCurTok->second != ")" ){
    if( auto arg = parse_expression() ){
      args.push_back( move( arg ) );
    } else {
      return nullptr;
    }

    if( mCurTok->second != "," && mCurTok->second != ")" ){
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

unique_ptr<expression> parser::parse_bin_op( int expPrec, unique_ptr<expression> lhs ){
  while( true ){
    int prec = getPrecedence();

    if( prec < expPrec ){
      return lhs;
    }

    string op = mCurTok->second;

    ++mCurTok;

    auto rhs = parse_primary();
    if( !rhs ){
      return nullptr;
    }

    int nextPrec = getPrecedence();
    if( prec < nextPrec ){
      rhs = parse_bin_op( prec + 1, move( rhs ) );
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

  if( auto E = parse_expression() ){
    return make_unique<func>( move( proto ), move( E ) );
  }

  return nullptr;
}

unique_ptr<prototype> parser::parse_extern(){
  ++mCurTok;

  return parse_proto();
}

unique_ptr<func> parser::parse_top(){
  if( auto E = parse_expression() ){
    auto proto = make_unique<prototype>( "", vector<string>() );

    return make_unique<func>( move( proto ), move( E ) );
  }

  return nullptr;
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
    cout << "ready> ";
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
