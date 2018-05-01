#ifndef __CALL_HH__
#define __CALL_HH__

#include<string>
#include<vector>
#include<memory>

#include<expression.hh>

class call : public expression{
private:
  std::string mCallee;
  std::vector<expr_ptr> mArgs;

public:
  call( const std::string& callee, std::vector<expr_ptr>&& args );

  /*!
   * @todo verify correct argument count(type?)
   */
  virtual
  std::string
  generate() const override;
};

#endif

