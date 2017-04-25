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

class parser{
private:
  std::vector<token> mTokens;
  decltype(mTokens)::iterator mCurTok;
  std::map<std::string, int> mOpPrecedence;

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
    mOpPrecedence( { { "<", 10 },
                     { ">", 10 },
                     { "+", 20 },
                     { "-", 20 },
                     { "*", 40 },
                     { "/", 40 }, } ){
  }

  void main_loop();
};

#endif
