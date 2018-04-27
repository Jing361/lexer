#ifndef __NUMBER_HH__
#define __NUMBER_HH__

#include<string>
#include<sstream>

#include"expression.hh"

class number : public expression{
private:
  double mValue;

public:
  number( double val );
  number( std::string val );

  virtual
  std::string
  generate() const override;
};

#endif

