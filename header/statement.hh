#ifndef __STATEMENT_HH__
#define __STATEMENT_HH__

#include<memory>

#include<expression.hh>

// statements are basically expressions that have a void type
// statements are also parsed slightly differently
class statement : public expression{
private:
  using base = expression;

public:
  statement();
};

using stmnt_ptr = std::unique_ptr<statement>;
//extern const decltype( std::make_unique<statement> )& make_stmnt;

#endif

