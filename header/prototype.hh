#ifndef __PROTOTYPE_HH__
#define __PROTOTYPE_HH__

#include<iterator>
#include<string>
#include<vector>

class prototype{
private:
  std::string mName;
  std::vector<std::string> mArgs;

public:
  template<typename inputIter>
  prototype( const std::string& name, inputIter first, inputIter last ):
    mName( name ),
    mArgs( std::make_move_iterator( first ), std::make_move_iterator( last ) ){
  }
};

#endif

