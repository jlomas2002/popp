#include <regex>

#include "tokenDef.hpp"
#include "lexer.hpp"

using namespace std;

string input = "";
int index;
int lineNum;

void initLexer(string filename){
  ifstream fileReader(filename);

  string line;
  while(getline (fileReader, line)){
          input.append(line);
      }

  fileReader.close();}

  index = 0;
  lineNum = 1;

}

Token getNextToken(){

    regex digit_regex("/\d/");
smatch match;

  if (regex_search(input.begin() + index, input.end(), match, digit_regex)) {
      Token tok;
      tok.lexeme = match.str();
      tok.type = digit_regex_regex;
      tok.lineNum = lineNum;
      return tok;
  }


}


Token peekNextToken(){
  int tmp = index;
  Token token;
  token = getNextToken();
  index = tmp;
  return token;
}

