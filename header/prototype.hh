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

using proto_ptr = unique_ptr<prototype>;
using make_proto = make_unique<prototype>;

#endif

