#ifndef TOKENISER_HPP
#define TOKENISER_HPP

#include <string>
#include <vector>

struct GrammarInfo { // holds info about grammar, e.g terminals, non terminals etc
  std::vector<std::string> nonTerminals;
};

struct Token { //tokens in a ebnf grammar file
  std::string lexeme;
  int lineNum;
  std::string error; //can change to enums?
  std::string type;
};

class Tokeniser {
public:
    void setGrammar(std::string grammar);
    void tokenise();
    GrammarInfo getGrammarInfo();
private:
    std::string grammar;
    int index;
    Token peekNextToken();
    Token getNextToken(); //change to make a token struct?
    GrammarInfo grammarInfo;
};

#endif