#include<iostream>
#include<string>

#include"parser.hh"

using namespace std;

int parser::getPrecedence(){
  try{
    return mOpPrecedence.at( mCurTok->tok );
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
  return make_unique<number>( stod( mCurTok++->tok ) );
}

unique_ptr<expression> parser::parse_paren(){
  ++mCurTok;
  auto E = parse_expression();

  if( !E ){
    return nullptr;
  }

  if( mCurTok->tok != ")" ){
    return log_error( "expected ')'" );
  }

  ++mCurTok;
  return E;
}

unique_ptr<expression> parser::parse_ident(){
  string name = mCurTok->tok;
  ++mCurTok;

  if( mCurTok->tok != "(" ){
    return make_unique<variable>( name );
  }
  ++mCurTok;

  vector<unique_ptr<expression> > args;
  bool bEndArgs = false;

  // if the '(' was found, it's a function call, parse arguments being passed
  while( mCurTok->tok != ")" && !bEndArgs ){
    if( auto arg = parse_expression() ){
      args.push_back( move( arg ) );
    } else {
      return nullptr;
    }

    if( mCurTok->tok == ")" ){
      bEndArgs = true;
    } else if( mCurTok->tok != "," ){
      return log_error( "Expected ')' or ',' in argument list; found: '" + mCurTok->tok + "'." );
    }

    ++mCurTok;
  }
  //there appears to be a bug that would allow func(arg,)
  // that is: a comma immediately followed by closing paren
  //  this is not intended, but not detrimental

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

  if( mCurTok->tok == "{" ){
    bBrackets = true;
    ++mCurTok;
  }

  do{
    if( auto E = parse_expression() ){
      body.emplace_back( move( E ) );
    }
  }while( bBrackets && mCurTok->tok != "}" );

  if( mCurTok->tok == "}" ){
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
}

unique_ptr<prototype> parser::parse_proto(){
  if( mCurTok->type != CLASS_IDENT ){
    return log_proto_error( "Expected function name in prototype." );
  }

  string fnName = mCurTok->tok;
  ++mCurTok;

  if( mCurTok->tok != "(" ){
    return log_proto_error( "Expected '(' in prototype." );
  }
  ++mCurTok;

  vector<string> args;
  if( mCurTok->type == CLASS_IDENT ){
    do{
      args.push_back( mCurTok->tok );
      ++mCurTok;
      if( mCurTok->tok == "," ){
        ++mCurTok;
      } else {
        break;
      }
    }while( true );
  }

  if( mCurTok->tok != ")" ){
    return log_proto_error( "Expected ')' after arguments in prototype." );
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

unique_ptr<expression> parser::parse_top(){
  return parse_ident();
/*  string protoName( mCurTok->tok );

  if( auto E = parse_expression() ){
    vector<unique_ptr<expression> > vec;
    auto proto = make_unique<prototype>( protoName, vector<string>() );

    vec.emplace_back( move( E ) );

    return make_unique<call>( move( proto ), move( vec ) );
  }

  return nullptr;
*/
}

unique_ptr<ifExpr> parser::parse_if(){
  ++mCurTok;

  auto P = parse_paren();
  auto T = parse_brackets();
  vector<unique_ptr<expression> > E;

  if( mCurTok->type == CLASS_ELSE ){
    ++mCurTok;

    E = parse_brackets();
  }

  return make_unique<ifExpr>( move( P ), move( T ), move( E ) );
}

unique_ptr<expression> parser::parse_primary(){
  if( mCurTok == mTokens.end() ){
    return log_error( "Reached early end of token stream." );
  }

  switch( mCurTok->type ){
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

  case CLASS_SEMI:
    ++mCurTok;
    return nullptr; 
  break;

  default:
    return log_error( "Unknown token: '" + mCurTok->tok + "' when expecting an expression." );
  break;
  }
}

void parser::handle_def(){
  if( auto D = parse_def() ){
    cout << "Parsed function def.\n";
    mDefs.emplace_back( move( D ) );
  } else {
    ++mCurTok;
  }
}

void parser::handle_extern(){
  if( auto E = parse_extern() ){
    cout << "Parsed extern.\n";
    mExterns.emplace_back( move( E ) );
  } else {
    ++mCurTok;
  }
}

void parser::handle_top(){
  if( auto T = parse_top() ){
    cout << "Parsed top.\n";
    mTopLevels.emplace_back( move( T ) );
  } else {
    ++mCurTok;
  }
}

void parser::main_loop(){
  bool not_eof = true;
  while( not_eof ){
    switch( mCurTok->type ){
    case CLASS_EOF:
      not_eof = false;
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

  cout << "DONE!" << endl;

  cout << "Listing definitions." << endl;
  for( unsigned int i = 0; i < mDefs.size(); ++i ){
    cout << mDefs[i]->name() << endl;
  }

  cout << "Listing externs." << endl;
  for( unsigned int i = 0; i < mExterns.size(); ++i ){
    cout << mExterns[i]->name() << endl;
  }
}

