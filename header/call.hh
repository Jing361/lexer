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

  /*!
   * @todo verify correct argument count
   */
  virtual std::string generate() override{
    std::string instructions;

    for( unsigned int i = 0; i < mArgs.size(); ++i ){
      instructions += mArgs[i]->generate();
    }

    return instructions + "call " + mCallee + "\n";
  }
};

#endif

