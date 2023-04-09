#ifndef TOKENISER_HPP
#define TOKENISER_HPP

#include <string>
#include <vector>

#include "tokenDef.hpp"

bool operator<(const Gtoken &a, const Gtoken &b);

extern std::set<char> reservedSymbols;

class Tokeniser {
public:
    Tokeniser(std::string grammar, std::string tokensInput);
    ~Tokeniser();
    Gtoken peekNextToken();
    Gtoken getNextToken();
    Gtoken makeErrorToken(Error err);
    std::vector<NonTerminalInfo> getAllNonTerminalInfo();
    std::set<std::string> getListOfTokens();
    std::vector<TokenRegex> getTokenRegexes();
private:
    void firstPass();
    void collectNonTerminalInfo();
    void parseExpr(NonTerminalInfo &ntInfo);
    void skipUntilBar();
    void collectListOfNonTerminals();
    void refineNonTerminalInfo(NonTerminalInfo &returnNtInfo, NonTerminalInfo &currentNtInfo);
    NonTerminalInfo findNonTerminalInfo(Gtoken t);
    int incrementIndex();
    Error checkTokOrNtError(std::string id);
    void extractRegexes(std::string input);

    std::vector<NonTerminalInfo> allNonTerminalInfo;
    std::set<std::string> listOfNonTerminals;
    std::set<std::string> listOfTokens;
    std::vector<TokenRegex> tokenRegexes;

    std::string grammar;

    int pos;
    int lineNum;
    int index;
    bool tokenFileExists;
    bool secondPass;
};

#endif
