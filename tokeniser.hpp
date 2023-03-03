#ifndef TOKENISER_HPP
#define TOKENISER_HPP

#include <string>
#include <vector>
#include <set>

struct Token { //tokens in a ebnf grammar file
  std::string lexeme;
  int lineNum;
  std::string error; //can change to enums?
  std::string type;
};

bool operator<(const Token &a, const Token &b);

struct startTerminals {
  std::string nonTerminal;
  std::set<Token> terminals;
};

extern std::set<char> keySymbols;

class Tokeniser {
public:
    Tokeniser(std::string grammar);
    ~Tokeniser();
    void tokenise();
    Token peekNextToken();
    Token getNextToken(); //change to make a token struct?
private:
    void collectStartTerminals();
    void refineStartTerminals(startTerminals &returnCollection, startTerminals &currentCollection);
    std::vector<startTerminals> nonTerminalInfo;
    std::string grammar;
    int index;
    int lineNum;
};

#endif