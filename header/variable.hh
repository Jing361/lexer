#ifndef __VARIABLE_HH__
#define __VARIABLE_HH__

#include<string>
#include<map>

#include"expression.hh"

class variable : public expression{
private:
  std::string mName;
  static std::map<std::string, unsigned int> tmVariables;
  static unsigned int tmNextAddress;

public:
  variable( const std::string& name );

  virtual std::string generate() const override;
};

#endif

