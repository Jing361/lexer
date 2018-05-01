#include<fstream>
#include<string>
#include<vector>
#include<iostream>

#include<lexer.hh>
#include<parser.hh>

using namespace std;

int main(){
  fstream file( "data/main.c" );
  vector<string> definitions;
  vector<string> topLevel;
  string text;
  string code( "goto\tstart\nnop\nnop\nnop\ngoto\tisr\n" );

  file.seekg( 0, ios::end );
  text.reserve( file.tellg() );
  file.seekg( 0, ios::beg );

  text.assign( istreambuf_iterator<char>( file ),
               istreambuf_iterator<char>() );

  lexer luthor( preprocess( text ) );

/*  cout << "original:" << endl;
  cout << text << endl;
  cout << endl;
  cout << "parsed:" << endl;
  for( auto token : luthor ){
    cout << token.lexeme << endl;
  }*/

  parser p( luthor.begin(), luthor.end() );
  p.parse_toplevel();

  return 0;
}

