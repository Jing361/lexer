#ifndef __FUNC_HH__
#define __FUNC_HH__

#include<memory>
#include<string>
#include<vector>

#include<expression.hh>
#include<prototype.hh>

class func : public statement{
private:
  prototype mProto;
  std::vector<expr_ptr> mBody;

public:
  func( prototype proto, std::vector<expr_ptr>&& body );

  std::string
  name() const;

  std::string
  generate() const;
};

#endif

