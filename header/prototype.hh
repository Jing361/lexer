#ifndef __PROTOTYPE_HH__
#define __PROTOTYPE_HH__

#include<string>
#include<vector>
#include<memory>

#include<statement.hh>

class prototype : public statement{
private:
  std::string mName;
  std::vector<type> mArgs;
  type mReturn;

public:
  prototype( type ret, const std::string& name, std::vector<type>&& args );

  std::string
  name() const;
};

using proto_ptr = std::unique_ptr<prototype>;
//extern const decltype( std::make_unique<prototype> )& make_proto;

#endif

