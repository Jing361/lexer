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
  std::vector<expression> mTopLevel;//'the program'

  int
  getPrecedence();

  std::unique_ptr<expression>
  log_error( const std::string& str );

  void
  handle_def();
  void
  handle_extern();
  void
  handle_top();

  expression
  parse_expression();



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

