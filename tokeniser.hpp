#ifndef TOKENISER_HPP
#define TOKENISER_HPP

#include <string>
#include <vector>

#include "tokenDef.hpp"

bool operator<(const Gtoken &a, const Gtoken &b);

extern std::set<char> reservedSymbols;

class Tokeniser {
public:
    Tokeniser(std::string grammar);
    ~Tokeniser();
    Gtoken peekNextToken();
    Gtoken getNextToken();
    Gtoken makeErrorToken(Error err);
    std::vector<TokenRegex> extractRegexes(std::string input);
    std::vector<StartTermCollection> getStartTermCollections();
private:
    void collectStartTerms();
    void refineStartTerms(StartTermCollection &returnCollection, StartTermCollection &currentCollection);
    StartTermCollection matchNonTerminalToCollection(Gtoken t);
    int incrementIndex();
    std::vector<StartTermCollection> startTermCollections;
    std::string grammar;
    int index;
    int pos;
    int lineNum;
};

#endif
