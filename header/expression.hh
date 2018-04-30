#ifndef __EXPRESSION_HH__
#define __EXPRESSION_HH__

#include<memory>
#include<string>

class expression{
private:
  std::string return_type;

public:
  expression( const std::string& type )
    : return_type( type ){
  }

  virtual ~expression() {}

  virtual
  std::string
  generate() const = 0;
};

using expr_ptr = std::unique_ptr<expression>;

#endif

