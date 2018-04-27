#ifndef __EXPRESSION_HH__
#define __EXPRESSION_HH__

#include<memory>
#include<string>

class expression{
private:
  std::string return_type;

public:
  virtual ~expression() {}

  virtual
  std::string
  generate() const = 0;
};

using expr_ptr = std::unique_ptr<expression>;

#endif

