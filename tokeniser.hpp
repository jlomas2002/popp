#ifndef TOKENISER_HPP
#define TOKENISER_HPP

#include <string>
#include <vector>

#include "tokenDef.hpp"

bool operator<(const Token &a, const Token &b);

extern std::set<char> keySymbols;

class Tokeniser {
public:
    Tokeniser(std::string grammar);
    ~Tokeniser();
    Token peekNextToken();
    Token getNextToken(); //change to make a token struct?
    std::vector<TokenRegexes> parseRegexes(std::string input);
    std::vector<startTerminals> getNonTerminalInfo();
private:
    void collectStartTerminals();
    void refineStartTerminals(startTerminals &returnCollection, startTerminals &currentCollection);
    std::vector<startTerminals> nonTerminalInfo;
    std::string grammar;
    int index;
    int lineNum;
};

#endif
