#ifndef TOKENISER_HPP
#define TOKENISER_HPP

#include <string>
#include <vector>

#include "tokenDef.hpp"

bool operator<(const Gtoken &a, const Gtoken &b);
bool operator==(const Gtoken &a, const Gtoken &b);

extern std::set<char> reservedSymbols;

class Tokeniser {
public:
    Tokeniser(std::string grammar, std::string tokensInput);
    ~Tokeniser();
    Gtoken peekNextToken();
    Gtoken getNextToken();
    Gtoken makeErrorToken(Error err, std::string lex);
    std::vector<FirstSetInfo> getAllFirstSetInfo();
    std::set<std::string> getListOfTokens();
    std::vector<TokenRegex> getTokenRegexes();
    bool getErrorState();
    Gtoken getTokenFileError();
    Gtoken getGrammarFileError();
private:
    void firstPass();
    void collectFirstSetInfo();
    void parseExpr(FirstSetInfo &firstSetInfo);
    void skipUntilBar();
    Gtoken collectListOfNonTerminals();
    void refineFirstSetInfo(FirstSetInfo &returnInfo, FirstSetInfo &currentInfo);
    FirstSetInfo findFirstSetInfo(Gtoken t);
    int incrementIndex(std::string input);
    Error checkUndefinedError(std::string id);
    Gtoken extractRegexes(std::string input);

    std::vector<FirstSetInfo> allFirstSetInfo;
    std::set<std::string> listOfNonTerminals;
    std::set<std::string> listOfTokens;
    std::vector<TokenRegex> tokenRegexes;

    std::string grammar;

    int pos;
    int lineNum;
    int index;
    bool tokenFileExists;
    bool secondPass;

    bool erroneous;
    Gtoken tokenFileError;
    Gtoken grammarFileError;
    
};

#endif
