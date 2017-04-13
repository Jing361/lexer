#ifndef __CALL_HH__
#define __CALL_HH__

#include<string>
#include<vector>
#include<iterator>

#include"expression.hh"

class call : public expression{
private:
  std::string mCallee;
  std::vector<std::unique_ptr<expression> > mArgs;

public:
  call( const std::string& callee, std::vector<std::unique_ptr<expression> >&& args ):
    mCallee( callee ),
    mArgs( std::move( args ) ){
  }
};

#endif

