
#ifndef LEXER_HPP
#define LEXER_HPP
#include <string>

struct Token {
  Type type;
  std::string lexeme;
  int lineNum;
  int pos;
  std::string error;
};

void initLexer();
Token getNextToken();
Token peekNextToken();

#endif

