#ifndef __PARSER_HH__
#define __PARSER_HH__

#include<vector>
#include<memory>
#include<map>
#include<string>

#include"call.hh"
#include"number.hh"
#include"variable.hh"
#include"binary.hh"
#include"prototype.hh"
#include"function.hh"
#include"shared.hh"
#include"ifExpr.hh"

class parser{
private:
  std::vector<token> mTokens;
  decltype( mTokens )::iterator mCurTok;
  std::map<std::string, int> mOpPrecedence;
  std::vector<std::unique_ptr<func> > mDefs;
  std::vector<expr_ptr> mTopLevel;//'the program'

  int
  getPrecedence();

  expr_ptr
  parse_expression();

  expr_ptr
  parse_primary();

  expr_ptr
  parse_paren();

  expr_ptr
  parse_binary( int lhs_precedence, expr_ptr lhs );

  expr_ptr
  parse_identifier();

  expr_ptr
  parse_branch();

  std::vector<expr_ptr>
  parse_brackets();

public:
//TODO: maybe comma should be operator with high precedence?
  template<typename inputIter>
  parser( inputIter first, inputIter last ):
    mTokens( first, last ),
    mCurTok( mTokens.begin() ),
    mOpPrecedence( { { "=",  10 },
                     { "+=", 10 }, { "-=", 10 },
                     { "/=", 10 }, { "*=", 10 },
                     { "||", 20 }, { "&&", 30 },
                     { "&",  40 }, { "|",  40 },
                     { "^",  50 },
                     { ">",  60 }, { "<",  60 },
                     { ">=", 60 }, { "<=", 60 },
                     { "+",  70 }, { "-",  70 },
                     { "*",  80 }, { "/",  80 }, } ){
  }

  void
  parse_toplevel();
};

#endif

