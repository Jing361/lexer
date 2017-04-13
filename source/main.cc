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

using namespace std;

typedef enum{
  NONE,
  IDENT,
  SYMBOL,
  INTEGER,
  SPACE,
  PAREN,
} classification;

typedef pair<classification, string> token;

class parser{
  vector<token> mTokens;
  decltype(mTokens)::iterator mCurTok;
  map<string, int> mOpPrecedence;

  int getPrecedence(){
    int prec = mOpPrecedence[mCurTok->second];

    if( prec < 0 ){
      return -1;
    } else {
      return prec;
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
    if( mCurTok->first != IDENT ){
      return log_proto_error( "Expected function name in prototype." );
    }

    string fnName = mCurTok->second;
    ++mCurTok;

    if( mCurtok->second == "(" ){
      return log_proto_error( "Expected '(' in prototype." );
    }

    vector<string> args;
    while( ( ++mCurTok )->first == IDENT ){
      args.push_back( mCurTok->second );
    }

    if( *mCurTok != ")" ){
      return log_proto_error( "Expected ')' in prototype." );
    }

    ++mCurTok;

    return make_unique<prototype>( fnName, move( args ) );
  }
  unique_ptr<function> parse_def(){
    ++mCurTok;

    auto proto = parse_proto();

    if( !proto ){
      return nullptr;
    }

    if( auto E = parse_expression() ){
      return make_unique<function>( move( proto ), move( E ) );
    }

    return nullptr;
  }
  unique_ptr<prototype> parse_extern(){
    ++mCurTok;

    return parse_proto();
  }
  unique_ptr<function> parse_top(){
    if( auto E = parse_expression() ){
      auto proto = make_unique<prototype>( "", vector<string>() );

      return make_unique<function>( move( proto ), move( E ) );
    }

    return nullptr;
  }

public:
  template<typename inputIter>
  parser( inputIter first, inputIter last ):
    mTokens( first, last ),
    mCurTok( first ),
    mOpPrecedence( { { "<", 10 },
                     { ">", 10 },
                     { "+", 20 },
                     { "-", 20 },
                     { "*", 40 },
                     { "/", 40 }, } ){
  }

  unique_ptr<expression> parse_primary(){
    switch( mCurTok->first ){
    case PAREN:
      return parse_paren();
    break;

    case IDENT:
      return parse_ident();
    break;

    case INTEGER:
      return parse_number();
    break;

    default:
      return log_error( "unknown token when expecting an expression." );
    break;
    }
  }
};

class lexer{
private:
  vector<token> mTokens;
  map<classification, function<bool(char)> > mClassDetect;

public:
  lexer():
    mClassDetect( { { NONE,    []( char ){ return false; } },
                    { SYMBOL,  []( char c ){ return ( ( c == '+' ) || ( c == '=' ) ); } },
                    { INTEGER, []( char c ){ return isdigit( c ); } },
                    { IDENT,   []( char c ){ return isalpha( c ); } },
                    { SPACE,   []( char c ){ return isspace( c ); } } } ){
  }

  void lex( const string& text ){
    for( unsigned int i = 0; i < text.size(); ++i ){
      string tok;
      classification cls = NONE;

      for( auto it : mClassDetect ){
        if( it.second( text[i] ) ){
          cls = it.first;
          break;
        }
      }

      while( mClassDetect[cls]( text[i] ) && ( i < text.size() ) ){
        tok += text[i++];
      }
      --i;

      if( cls != SPACE ){
        mTokens.emplace_back( cls, tok );
      }
    }
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
  string text( "xxx = yz + 301" );

  lexer luthor;
  luthor.lex( text );

  parser p( luthor.begin(), luthor.end() );
  p.parse_primary();

  return 0;
}

