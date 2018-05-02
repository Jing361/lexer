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

using var_ptr = std::unique_ptr<variable>;
//extern const decltype( make_unique<variable> )& make_var;

#endif

