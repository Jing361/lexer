#ifndef __LEXER_HH__
#define __LEXER_HH__

#include<vector>
#include<functional>
#include<string>
#include<utility>
#include<map>

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

class lexer{
public:
  typedef std::vector<token> vec_token;

private:
  std::vector<token> mTokens;
  std::map<classification, std::function<bool(char)> > mClassDetect;

public:
  lexer();

  void lex( const std::string& text );

  vec_token::const_iterator begin() const;
  vec_token::const_iterator end() const;

  vec_token::iterator begin();
  vec_token::iterator end();
};

#endif
