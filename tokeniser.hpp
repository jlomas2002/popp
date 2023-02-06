#ifndef TOKENISER_HPP
#define TOKENISER_HPP

#include <string>
#include <vector>

class Tokeniser {
public:
    Tokeniser();
    ~Tokeniser();
private:
    std::vector<std::string> grammar;
    std::string getNextToken();
};

#endif