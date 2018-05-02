#ifndef __CALL_HH__
#define __CALL_HH__

#include<string>
#include<vector>
#include<memory>

#include<expression.hh>

class call : public expression{
private:
  using base = expression;

  std::string mCallee;
  std::vector<expr_ptr> mArgs;

public:
  call( const std::string& callee, std::vector<expr_ptr>&& args, const std::string& ret_type );

  /*!
   * @todo verify correct argument count(type?)
   */
  virtual
  std::string
  generate() const override;
};

using call_ptr = std::unique_ptr<call>;

#endif

