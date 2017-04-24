#include<cctype>
#include<string>
#include<map>
#include<vector>

#include"lexer.hh"
#include"parser.hh"

using namespace std;

int main(){
  string text( "def foo( x y ) x+y;\nfoo( 1, 2 );\n" );

  lexer luthor;
  luthor.lex( text );

  parser pansy( luthor.begin(), luthor.end() );
  pansy.main_loop();

  return 0;
}

