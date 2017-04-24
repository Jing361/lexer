#include<cctype>
#include<string>
#include<functional>
#include<map>
#include<vector>
#include<utility>

#include<iostream>

#include<memory>

#include"call.hh"
#include"number.hh"
#include"variable.hh"
#include"binary.hh"
#include"prototype.hh"
#include"function.hh"

using namespace std;

typedef enum{
  CLASS_NONE,
  CLASS_IDENT,
  CLASS_SYMBOL,
  CLASS_INTEGER,
  CLASS_SPACE,
  CLASS_PAREN,
  CLASS_EOF,
  CLASS_SEMI,
  CLASS_DEF,
  CLASS_EXTERN,
} classification;

typedef pair<classification, string> token;

class parser{
  vector<token> mTokens;
  decltype(mTokens)::iterator mCurTok;
  map<string, int> mOpPrecedence;

  int getPrecedence(){
    try{
      return mOpPrecedence.at( mCurTok->second );
    }catch( out_of_range ){
      return -1;
    }
  }

  unique_ptr<expression> log_error( const string& str ){
    cout << "Log Error: " << str << endl;
    return nullptr;
  }
  unique_ptr<prototype> log_proto_error( const string& str ){
    cout << "Log Error: " << str << endl;
    return nullptr;
  }

  std::unique_ptr<number> parse_number(){
    double value;
    std::stringstream ss( mCurTok->second );

    ss >> value;

    return std::make_unique<number>( value );
  }
  unique_ptr<expression> parse_paren(){
    ++mCurTok;
    auto V = parse_expression();

    if( !V ){
      return nullptr;
    }

    if( mCurTok->second != ")" ){
      return log_error( "expected ')'" );
    }

    ++mCurTok;
    return V;
  }
  unique_ptr<expression> parse_ident(){
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

      if( mCurTok->second != "," || mCurTok->second != ")" ){
        return log_error( "Expected ')' or ',' in argument list" );
      }

      ++mCurTok;
    }

    return make_unique<call>( name, move( args ) );
  }
  unique_ptr<expression> parse_expression(){
    auto lhs = parse_primary();

    if( !lhs ){
      return nullptr;
    }

    return parse_bin_op( 0, move( lhs ) );
  }
  unique_ptr<expression> parse_bin_op( int expPrec, unique_ptr<expression> lhs ){
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
  unique_ptr<prototype> parse_proto(){
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
  unique_ptr<func> parse_def(){
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
  unique_ptr<prototype> parse_extern(){
    ++mCurTok;

    return parse_proto();
  }
  unique_ptr<func> parse_top(){
    if( auto E = parse_expression() ){
      auto proto = make_unique<prototype>( "", vector<string>() );

      return make_unique<func>( move( proto ), move( E ) );
    }

    return nullptr;
  }

  unique_ptr<expression> parse_primary(){
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
  void handle_def(){
    if( parse_def() ){
      cout << "Parsed function def.\n";
    } else {
      ++mCurTok;
    }
  }
  void handle_extern(){
    if( parse_extern() ){
      cout << "Parsed extern.\n";
    } else {
      ++mCurTok;
    }
  }
  void handle_top(){
    if( parse_top() ){
      cout << "Parsed top.\n";
    } else {
      ++mCurTok;
    }
  }

public:
  template<typename inputIter>
  parser( inputIter first, inputIter last ):
    mTokens( first, last ),
    mCurTok( mTokens.begin() ),
    mOpPrecedence( { { "<", 10 },
                     { ">", 10 },
                     { "+", 20 },
                     { "-", 20 },
                     { "*", 40 },
                     { "/", 40 }, } ){
  }

  void main_loop(){
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
};

class lexer{
private:
  vector<token> mTokens;
  map<classification, function<bool(char)> > mClassDetect;

public:
  lexer():
    mClassDetect( { { CLASS_NONE,    []( char  ){ return false; } },
                    { CLASS_SYMBOL,  []( char c ){ return ( ( c == '+' ) || ( c == '=' ) || ( c == '-' ) ); } },
                    { CLASS_INTEGER, []( char c ){ return isdigit( c ); } },
                    { CLASS_IDENT,   []( char c ){ return isalpha( c ); } },
                    { CLASS_SPACE,   []( char c ){ return isspace( c ); } },
                    //TODO: perhaps this should only detect '('?
                    { CLASS_PAREN,   []( char c ){ return c == '(' || c == ')'; } },
                    { CLASS_EOF,     []( char c ){ return c == -1; } },
                    { CLASS_SEMI,    []( char c ){ return c == ';'; } } } ){
  }

  void lex( const string& text ){
    for( unsigned int i = 0; i < text.size(); ++i ){
      string tok;
      classification cls = CLASS_NONE;

      for( auto it : mClassDetect ){
        if( it.second( text[i] ) ){
          cls = it.first;
          break;
        }
      }

      if( cls == CLASS_NONE ){
        cout << "Invalid character: '" << text[i] << "'." << endl;
        continue;
      }

      while( mClassDetect[cls]( text[i] ) && ( i < text.size() ) ){
        tok += text[i++];
      }
      --i;

      if( cls == CLASS_IDENT ){
        if( tok == "def" ){
          cls = CLASS_DEF;
        } else if( tok == "extern" ){
          cls = CLASS_EXTERN;
        }
      }

      if( cls != CLASS_SPACE && tok != "" ){
        mTokens.emplace_back( cls, tok );
      }
    }

    mTokens.emplace_back( CLASS_EOF, "" );
  }

  const auto begin() const{
    return mTokens.begin();
  }
  const auto end() const{
    return mTokens.end();
  }

  auto begin(){
    return mTokens.begin();
  }
  auto end(){
    return mTokens.end();
  }
};

int main(){
  string text( "def foo( x y ) x+y;\n" );

  lexer luthor;
  luthor.lex( text );

  parser pansy( luthor.begin(), luthor.end() );
  pansy.main_loop();

  return 0;
}

