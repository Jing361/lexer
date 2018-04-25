#ifndef __SHARED_TYPES_HH__
#define __SHARED_TYPES_HH__

#include<utility>
#include<string>
#include<set>

enum class{
  NONE,
  IDENTIFIER,
  NUMBER,
  EoF,
  SEMI,
  COMMA,
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
  MINUS,
  PLUS,
  STAR,
  SLASH,
} classification;

struct token{
  classification type;
  std::string lexeme;
  std::pair<unsigned long, unsigned long> location;// row, column

  token( classification cl, const std::string& s, unsigned long r = 0, unsigned long co = 0 )
    : type( cl )
    , tok( s )
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

