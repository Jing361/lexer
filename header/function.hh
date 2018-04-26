#ifndef __FUNC_HH__
#define __FUNC_HH__

#include<memory>
#include<string>
#include<vector>

#include"expression.hh"
#include"prototype.hh"

class func{
private:
  prototype mProto;
  std::vector<expression> mBody;

public:
  func( prototype proto, std::vector<expression> body );

  std::string
  name() const;

  std::string
  generate() const;
};

#endif

