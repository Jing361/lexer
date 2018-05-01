#include<stdexcept>

#include"lexer.hh"

using namespace std;

lexer::lexer( const std::string& text )
  : mText( text ){
  mIter = mText.begin();
  lex();
}

void
lexer::lex(){
  int line_number = 1;

  while( mIter != mText.end() ){
    if( *mIter == ' ' || *mIter == '\r' || *mIter == '\n' ){
      if( *mIter == '\r' || *mIter == '\n' ){
        ++line_number;
      }

      ++mIter;

      continue;
    } else if( is_digit( *mIter ) ){
      mTokens.emplace_back( classification::NUMBER, process_number(), line_number );
    } else if( is_alpha( *mIter ) || *mIter == '_' ){
      string lexeme( 1, *mIter++ );

      while( is_alpha( *mIter ) || *mIter == '_' || is_digit( *mIter ) ){
        lexeme.push_back( *mIter++ );
      }

      if( lexeme == "if" ){
        mTokens.emplace_back( classification::IF, lexeme, line_number );
      } else if( lexeme == "else" ){
        mTokens.emplace_back( classification::ELSE, lexeme, line_number );
      } else if( lexeme == "extern" ){
        mTokens.emplace_back( classification::EXTERN, lexeme, line_number );
      } else if( lexeme == "typedef" ){
        mTokens.emplace_back( classification::TYPEDEF, lexeme, line_number );
      } else {
        mTokens.emplace_back( classification::IDENTIFIER, lexeme, line_number );
      }
    } else if( *mIter == '"' ){
      string lexeme( 1, *mIter++ );

      while( *mIter != '"' ){
        lexeme.push_back( *mIter++ );
      }
      ++mIter;

      mTokens.emplace_back( classification::STRING, lexeme, line_number );
    } else if( *mIter == ',' ){
      mTokens.emplace_back( classification::COMMA,    string( 1, *mIter++ ), line_number );
    } else if( *mIter == '*' ){
      mTokens.emplace_back( classification::STAR,     string( 1, *mIter++ ), line_number );
    } else if( *mIter == '/' ){
      mTokens.emplace_back( classification::SLASH,    string( 1, *mIter++ ), line_number );
    } else if( *mIter == '+' ){
      mTokens.emplace_back( classification::PLUS,     string( 1, *mIter++ ), line_number );
    } else if( *mIter == '-' ){
      mTokens.emplace_back( classification::MINUS,    string( 1, *mIter++ ), line_number );
    } else if( *mIter == '(' ){
      mTokens.emplace_back( classification::LPAREN,   string( 1, *mIter++ ), line_number );
    } else if( *mIter == ')' ){
      mTokens.emplace_back( classification::RPAREN,   string( 1, *mIter++ ), line_number );
    } else if( *mIter == '[' ){
      mTokens.emplace_back( classification::LBRACKET, string( 1, *mIter++ ), line_number );
    } else if( *mIter == ']' ){
      mTokens.emplace_back( classification::RBRACKET, string( 1, *mIter++ ), line_number );
    } else if( *mIter == '{' ){
      mTokens.emplace_back( classification::LBRACE,   string( 1, *mIter++ ), line_number );
    } else if( *mIter == '}' ){
      mTokens.emplace_back( classification::RBRACE,   string( 1, *mIter++ ), line_number );
    } else if( *mIter == ';' ){
      mTokens.emplace_back( classification::SEMI,   string( 1, *mIter++ ), line_number );
    } else if( *mIter == '=' ){
      mTokens.emplace_back( classification::EQUAL,   string( 1, *mIter++ ), line_number );
    } else {
      throw runtime_error( string( "Invalid token: " ) + string( 1, *mIter ) );
    }
  }
}

string
lexer::process_number(){
  string lexeme( 1, *mIter++ );
  int decimal_count = 0;
  bool stop = false;

  do{
    if( *mIter == '.' ){
      if( decimal_count++ < 1 ){
        lexeme.push_back( *mIter++ );
      } else {
        throw runtime_error( "Number can only have one decimal place" );
      }
    } else if( is_digit( *mIter ) /*|| *mIter == 'e'*/ ){
      lexeme.push_back( *mIter++ );
    } else {
      stop = true;
    }
  } while( !stop );

  return lexeme;
}

lexer::vec_token::const_iterator lexer::cbegin() const{
  return mTokens.cbegin();
}

lexer::vec_token::const_iterator lexer::cend() const{
  return mTokens.cend();
}

lexer::vec_token::iterator lexer::begin(){
  return mTokens.begin();
}

lexer::vec_token::iterator lexer::end(){
  return mTokens.end();
}

