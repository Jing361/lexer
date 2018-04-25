#include"lexer.hh"

using namespace std;

lexer::lexer( const std::string& text )
  : mText( text ){
  mIter = mText.begin();
  lex();
}

lexer::lex(){
  while( mIter != mText.end() ){
    if( is_digit( *mIter ) ){
      mTokens.emplace_back( classification::NUMBER, process_number() );
    } else if( is_alpha( *mIter ) ){
      string lexeme( 1, *mIter++ );

      while( is_alpha( *mIter ) ){
        lexeme.push_back( *mIter++ );
      }

      if( lexeme == "if" ){
        mTokens.emplace_back( classification::IF, lexeme );
      } else if( lexeme == "else" ){
        mTokens.emplace_back( classification::ELSE, lexeme );
      } else if( lexeme == "extern" ){
        mTokens.emplace_back( classification::EXTERN, lexeme );
      } else {
        mTokens.emplace_back( classification::IDENTIFIER, lexeme );
      }
    } else if( *mIter == '*' ){
      mTokens.emplace_back( classification::STAR, string( 1, *mIter++ ) );
    } else if( *mIter == '/' ){
      mTokens.emplace_back( classification::SLASH, string( 1, *mIter++ ) );
    } else if( *mIter == '+' ){
      mTokens.emplace_back( classification::PLUS, string( 1, *mIter++ ) );
    } else if( *mIter == '-' ){
      mTokens.emplace_back( classification::MINUS, string( 1, *mIter++ ) );
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
        throw error;
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

