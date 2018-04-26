#ifndef __EXPRESSION_HH__
#define __EXPRESSION_HH__

#include<string>

class expression{
private:
  std::string return_type;

public:
  virtual ~expression() {}

  virtual
  std::string
  generate() const{}
};

#endif

