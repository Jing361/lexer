#ifndef __VARIABLE_HH__
#define __VARIABLE_HH__

#include<memory>
#include<string>
#include<map>

#include<expression.hh>
#include<type.hh>

class variable : public expression{
private:
  std::string mName;
  type mType;

public:
  variable( type t, const std::string& name );

  virtual
  std::string
  generate() const override;
};

using var_ptr = unique_ptr<variable>;
using make_var = make_unique<variable>;

#endif

