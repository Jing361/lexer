#ifndef __SHARED_TYPES_HH__
#define __SHARED_TYPES_HH__

#include<utility>
#include<string>

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
  CLASS_COMMA,
} classification;

typedef std::pair<classification, std::string> token;

#endif

