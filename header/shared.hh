#ifndef __SHARED_TYPES_HH__
#define __SHARED_TYPES_HH__

#include<utility>
#include<string>

typedef enum{
  CLASS_NONE,
  CLASS_IDENT,
  CLASS_OPERATOR,
  CLASS_INTEGER,
  CLASS_SPACE,
  CLASS_PAREN,
  CLASS_EOF,
  CLASS_SEMI,
  CLASS_DEF,
  CLASS_EXTERN,
  CLASS_COMMA,
  CLASS_BRACKET,
  CLASS_IF,
  CLASS_ELSE,
} classification;

struct token{
  classification type;
  std::string tok;
  unsigned long row;
  unsigned long column;

  token( classification c, const std::string& s, unsigned long r, unsigned long o ):
    type( c ),
    tok( s ),
    row( r ),
    column( o ){
  }
};

#endif

