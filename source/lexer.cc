#include<iostream>

#include"lexer.hh"

using namespace std;

lexer::lexer():
  //TODO: perhaps should only detect '('?
  mClassDetect( { { CLASS_NONE,     []( char   ){ return false; } },
                  { CLASS_OPERATOR, []( char c ){ return ( ( c == '+' ) ||
                                                           ( c == '-' ) ||
                                                           ( c == '=' ) ||
                                                           ( c == '<' ) ||
                                                           ( c == '>' ) ||
                                                           ( c == '|' ) ||
                                                           ( c == '&' ) ||
                                                           ( c == '*' ) ||
                                                           ( c == '/' ) ); } },
                  { CLASS_INTEGER,  []( char c ){ return isdigit( c ); } },
                  { CLASS_IDENT,    []( char c ){ return isalpha( c ); } },
                  { CLASS_SPACE,    []( char c ){ return isspace( c ); } },
                  { CLASS_PAREN,    []( char c ){ return c == '(' || c == ')'; } },
                  { CLASS_EOF,      []( char c ){ return c == -1; } },
                  { CLASS_SEMI,     []( char c ){ return c == ';'; } },
                  { CLASS_BRACKET,  []( char c ){ return ( ( c == '{' ) || ( c == '}' ) ); } },
                  { CLASS_COMMA,    []( char c ){ return c == ','; } } } ){
}

void lexer::lex( const string& text ){
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
      } else if( tok == "if" ){
        cls = CLASS_IF;
      } else if( tok == "else" ){
        cls = CLASS_ELSE;
      }
    }

    if( cls != CLASS_SPACE && tok != "" ){
      mTokens.emplace_back( cls, tok );
    }
  }

  mTokens.emplace_back( CLASS_EOF, "" );
}

lexer::vec_token::const_iterator lexer::begin() const{
  return mTokens.cbegin();
}
lexer::vec_token::const_iterator lexer::end() const{
  return mTokens.cend();
}

lexer::vec_token::iterator lexer::begin(){
  return mTokens.begin();
}
lexer::vec_token::iterator lexer::end(){
  return mTokens.end();
}

