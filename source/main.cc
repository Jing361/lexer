#include<cctype>
#include<string>
#include<functional>
#include<map>
#include<vector>
#include<utility>

#include<iostream>

using namespace std;

typedef enum{
  NONE,
  IDENTIFIER,
  SYMBOL,
  INTEGER,
  SPACE,
} classification;

int main(){
  typedef pair<classification, string> token;
  string text( "xxx = yz + 301" );
  vector<token> tokens;
  map<classification, function<bool(char)> > classDef;

  classDef[NONE] =       []( char ){ return false; };
  classDef[SYMBOL] =     []( char c ){ return ( ( c == '+' ) || ( c == '=' ) ); };
  classDef[INTEGER] =    []( char c ){ return isdigit( c ); };
  classDef[IDENTIFIER] = []( char c ){ return isalpha( c ); };
  classDef[SPACE] =      []( char c ){ return isspace( c ); };

  for( unsigned int i = 0; i < text.size(); ++i ){
    string tok;
    classification cls = NONE;

    for( auto it : classDef ){
      if( it.second( text[i] ) ){
        cls = it.first;
        break;
      }
    }

    while( classDef[cls]( text[i] ) && ( i < text.size() ) ){
      tok += text[i++];
    }
    --i;
    if( cls != SPACE ){
      tokens.emplace_back( cls, tok );
    }
  }

  for( auto it : tokens ){
    cout << it.first << '\t' << it.second << endl;
  }

  return 0;
}

