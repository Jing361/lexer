#include<fstream>
#include<string>
#include<vector>
#include<iostream>

#include"lexer.hh"
#include"parser.hh"

using namespace std;

int main(){
  fstream file( "data/main.c" );
  vector<string> definitions;
  vector<string> topLevel;
  string text;
  string code;

  file.seekg( 0, ios::end );
  text.reserve( file.tellg() );
  file.seekg( 0, ios::beg );

  text.assign( istreambuf_iterator<char>( file ),
               istreambuf_iterator<char>() );

  lexer luthor;
  luthor.lex( text );

  parser pansy( luthor.begin(), luthor.end() );
  pansy.main_loop();

  cout << "generating code" << endl;
  cout << "\tdefinitions" << endl;
  pansy.get_defs( back_inserter( definitions ) );
  cout << "\ttoplevel" << endl;
  pansy.get_top( back_inserter( topLevel ) );

  cout << "concatenating code" << endl;
  cout << "\ttoplevel" << endl;
  for( auto top : topLevel ){
    code += top;
  }

  cout << "\tdefinitions" << endl;
  for( auto def : definitions ){
    code += def;
  }

  cout << code << endl;

  return 0;
}

