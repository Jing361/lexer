#ifndef __SHARED_TYPES_HH__
#define __SHARED_TYPES_HH__

#include<utility>
#include<string>
#include<set>

enum class classification{
  NONE,
  IDENTIFIER,
  NUMBER,
  EoF,
  SEMI,
  COMMA,
  STRING,
  LPAREN,//(
  RPAREN,//)
  LBRACE,//{
  RBRACE,//}
  LBRACKET,//[
  RBRACKET,//]
  IF,
  ELSE,
  EXTERN,
  TYPE,
  TYPEDEF,
  MINUS,
  PLUS,
  STAR,
  SLASH,
  EQUAL,
};

struct token{
  classification type;
  std::string lexeme;
  std::pair<unsigned long, unsigned long> location;// row, column

  token( classification cl, const std::string& s, unsigned long r = 0, unsigned long co = 0 )
    : type( cl )
    , lexeme( s )
    , location( r, co ){
  }
};

bool
is_digit( char c );

bool
is_alpha( char c );

std::string
preprocess( const std::string& str );

#endif

