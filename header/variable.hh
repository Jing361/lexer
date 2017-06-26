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
  variable( const std::string& name ):
    mName( name ){
  }

  virtual std::string generate() override{
    //TODO variables have 2 primary uses.  read and write.
    // how to discern between them?
    return "PLACEHOLDER\n";
  }
};

#endif

