#include<type.hh>

using namespace std;

type::type( const string& name, const set<qualifiers>& quals )
  : mTypename( name )
  , mQualifiers( quals ){
}

std::string
type::type_name() const{
  return mTypename;
}

//const decltype( std::make_unique<type> )& = std::make_unique<type>;

