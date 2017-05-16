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
  std::vector<std::unique_ptr<func> > mTopLevels;
  std::vector<std::unique_ptr<prototype> > mExterns;

  int getPrecedence();

  std::unique_ptr<expression> log_error( const std::string& str );
  std::unique_ptr<prototype> log_proto_error( const std::string& str );

  std::unique_ptr<number> parse_number();
  std::unique_ptr<expression> parse_paren();
  std::unique_ptr<expression> parse_ident();
  std::unique_ptr<expression> parse_expression();
  std::unique_ptr<expression> parse_bin_op( int expPrec, std::unique_ptr<expression> lhs );
  std::unique_ptr<prototype> parse_proto();
  std::unique_ptr<func> parse_def();
  std::unique_ptr<prototype> parse_extern();
  std::unique_ptr<func> parse_top();
  std::unique_ptr<ifExpr> parse_if();
  std::vector<std::unique_ptr<expression> > parse_brackets();

  std::unique_ptr<expression> parse_primary();
  void handle_def();
  void handle_extern();
  void handle_top();

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

  void main_loop();
};

#endif

