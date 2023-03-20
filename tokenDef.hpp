#ifndef TOKENDEF_HPP
#define TOKENDEF_HPP

#include <string>
#include <set>

struct Token { //tokens in a ebnf grammar file
  std::string lexeme;
  int lineNum;
  std::string error; //can change to enums?
  std::string type;
};

struct startTerminals {
  std::string nonTerminal;
  std::set<Token> terminals;
};

struct TokenRegexes {
  std::string name;
  std::string regex;
};

#endif