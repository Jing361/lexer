#ifndef __PROTOTYPE_HH__
#define __PROTOTYPE_HH__

#include<string>
#include<vector>

class prototype{
private:
  std::string mName;
  std::vector<std::string> mArgs;

public:
  prototype( const std::string& name, std::vector<std::string>&& args );

  std::string name() const;
};

#endif

