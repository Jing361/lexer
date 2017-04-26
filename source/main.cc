#include<fstream>
#include<string>

#include"lexer.hh"
#include"parser.hh"

using namespace std;

int main(){
  fstream file( "data/main.c" );
  string text;

  file.seekg( 0, ios::end );
  text.reserve( file.tellg() );
  file.seekg( 0, ios::beg );

  text.assign( istreambuf_iterator<char>( file ),
               istreambuf_iterator<char>() );

  lexer luthor;
  luthor.lex( text );

  parser pansy( luthor.begin(), luthor.end() );
  pansy.main_loop();

  return 0;
}

