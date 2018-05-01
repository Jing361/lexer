#ifndef __STATEMENT_HH__
#define __STATEMENT_HH__

#include<expression.hh>

// statements are basically expressions that have a void type
// statements are also parsed slightly differently
class statement : public expression{
public:
  statement()
    : expression( "void" ){
  }
};

using stmnt_ptr = unique_ptr<statement>;
using make_stmnt = make_unique<statement>;

#endif

