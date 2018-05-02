#ifndef __TYPE_HH__
#define __TYPE_HH__

#include<memory>
#include<string>
#include<set>

using qualifier = std::string;

class type{
private:
  std::string mTypename;
  std::set<qualifier> mQualifiers;

public:
  type( const std::string& name,
        const std::set<qualifier>& quals = std::set<qualifier>{} );

  std::string
  type_name() const;
};

//using type_ptr = std::unique_ptr<type>;
//extern const decltype( std::make_unique<type> )& make_type;

#endif

