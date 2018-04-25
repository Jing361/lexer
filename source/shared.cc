#include<shared.hh>

using namespace std;

bool
is_digit( char c ){
  return int(c) >= '0'
      && int(c) <= '9';
}

bool
is_alpha( char c ){
  return int(c) >= 'A'
      && int(c) <= 'z';
}

string
preprocess( const string& str ){
  string ret;

  for( unsigned int i = 0; i < str.size(); ++i ){
    if( str[i] == ' '
     || str[i] == '\r'
     || str[i] == '\n' ){
      continue;
    }

    if( str[i] == '/' ){
      if( str[i + 1] == '/' ){
        i += 2;
        while( !( str[i] == '\r' && str[i] == '\n' ) ){
          ++i;
        }
      } else if( str[i + 1] == '*' ){
        i += 2;
        while( !( stri[i] == '*' && str[i + 1] == '/' ) ){
          ++i;
        }
      }
    } else if( str[i] == '#' ){
    } else {
      ret.push_back( str[i] );
    }
  }

  return ret;
}

