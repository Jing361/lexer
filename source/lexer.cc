#include<iostream>

#include"lexer.hh"

using namespace std;

static classification classify( char c ){
  if( ( c == '-' ) ||
      ( c == '=' ) ||
      ( c == '<' ) ||
      ( c == '>' ) ||
      ( c == '|' ) ||
      ( c == '&' ) ||
      ( c == '*' ) ||
      ( c == '/' ) ){
    return classification::OPERATOR;
  } else if( isdigit( c ) ){
    return classification::INTEGER;
  } else if( isalpha( c ) ){
    return classification::IDENT;
  } else if( isspace( c ) ){
    return classification::SPACE;
  } else if( c == '(' || c == ')' ){
    return classification::PAREN;
  } else if( c == -1 ){
    return classification::EoF;
  } else if( c == ';' ){
    return classification::SEMI;
  } else if( ( c == '{' ) || ( c == '}' ) )
    return classification::BRACKET;
  } else if( c == ',' ){
    return classification::COMMA;
  } else {
    return classification::NONE;
  }
}

lexer::lexer():
  //TODO: perhaps should only detect '('?
  mClassDetect( { { classification::NONE,     []( char   ){ return false; } },
                  { classification::OPERATOR, []( char c ){ return ( ( c == '+' ) ||
                                                                     ( c == '-' ) ||
                                                                     ( c == '=' ) ||
                                                                     ( c == '<' ) ||
                                                                     ( c == '>' ) ||
                                                                     ( c == '|' ) ||
                                                                     ( c == '&' ) ||
                                                                     ( c == '*' ) ||
                                                                     ( c == '/' ) ); } },
                  { classification::INTEGER,  []( char c ){ return isdigit( c ); } },
                  { classification::IDENT,    []( char c ){ return isalpha( c ); } },
                  { classification::SPACE,    []( char c ){ return isspace( c ); } },
                  { classification::PAREN,    []( char c ){ return c == '(' || c == ')'; } },
                  { classification::EoF,      []( char c ){ return c == -1; } },
                  { classification::SEMI,     []( char c ){ return c == ';'; } },
                  { classification::BRACKET,  []( char c ){ return ( ( c == '{' ) || ( c == '}' ) ); } },
                  { classification::COMMA,    []( char c ){ return c == ','; } } } ){
}

void lexer::lex( const string& text ){
  unsigned long row = 0;
  unsigned long column = 0;

  for( unsigned int i = 0; i < text.size(); ++i ){
    string tok;
    classification cls = classification::NONE;
    ++column;

    cls = classifiy( it.second( text[i] );

    if( cls == classification::NONE ){
      cout << "Invalid character: '" << text[i] << "'." << endl;
      continue;
    }

    while( mClassDetect[cls]( text[i] ) && ( i < text.size() ) ){
      tok += text[i++];
      ++column;
    }
    --i;
    --column;

    if( cls == classification::IDENT ){
      if( types.count( tok ) ){
        cls = classification::TYPE;
      } else if( tok == "extern" ){
        cls = classification::EXTERN;
      } else if( tok == "if" ){
        cls = classification::IF;
      } else if( tok == "else" ){
        cls = classification::ELSE;
      }
    }

    if( cls != classification::SPACE && tok != "" ){
      mTokens.emplace_back( cls, tok, row, column );
    } else if( !tok.empty() && ( tok[0] == '\n' || tok[0] == '\r' ) ){
      column = 0;
      ++row;
    }
  }

  mTokens.emplace_back( EoF, "", row, column );
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

