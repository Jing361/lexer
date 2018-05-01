#ifndef __PARSER_HH__
#define __PARSER_HH__

#include<vector>
#include<memory>
#include<map>
#include<string>

#include<type.hh>
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

  stmnt_ptr
  parse_prog_statement();

  stmnt_ptr
  parse_variable_declaration();

  stmnt_ptr
  parse_function_declaration();

  type_ptr
  parse_type();

  stmnt_ptr
  parse_qualifier();

  stmnt_ptr
  parse_typename();

  stmnt_ptr
  parse_type_spec();

  stmnt_ptr
  parse_identifier();

  stmnt_ptr
  parse_initialization();

  stmnt_ptr
  parse_param_list();

  type
  parse_param();

  stmnt_ptr
  parse_integer();

  stmnt_ptr
  parse_number();

  expr_ptr
  parse_value();

  stmnt_ptr
  parse_statement();

  std::vector<expr_ptr>
  parse_arg_list();

  expr_ptr
  parse_arg();

  stmnt_ptr
  parse_function_definition();

  stmnt_ptr
  parse_branch();

  stmnt_ptr
  parse_for();

  stmnt_ptr
  parse_while();

  std::vector<expr_ptr>
  stmnt_ptr
  parse_block();

  expr_ptr
  parse_expression();

  expr_ptr
  parse_binary( int lhs_precedence, expr_ptr lhs );

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

