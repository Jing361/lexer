#ifndef __TYPE_HH__
#define __TYPE_HH__

#include<memory>
#include<string>
#include<set>

#include<statement.hh>

using qualifier = std::string;

class type : public statement{
private:
  std::string mTypename;
  std::set<qualifer> mQualifiers;

public:
  type( const std::string& name,
        const std::set<qualifier>& quals = std::set<qualifer>{} );

  std::string
  type_name() const;
};

using type_ptr = unique_ptr<type>;
using make_type = make_unique<type>;

#endif

