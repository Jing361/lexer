#ifndef __LEXER_HH__
#define __LEXER_HH__

#include<vector>
#include<string>

#include"shared.hh"

class lexer{
public:
  typedef std::vector<token> vec_token;

private:
  vec_token mTokens;
  std::string mText;
  std::string::iterator mIter;

  void lex();

  std::string
  process_number();

public:
  lexer( const std::string& text );

  vec_token::const_iterator cbegin() const;
  vec_token::const_iterator cend() const;

  vec_token::iterator begin();
  vec_token::iterator end();
};

#endif

