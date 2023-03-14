#ifndef WRITER_HPP
#define WRITER_HPP
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "tokenDef.hpp"


struct SubsequentCodeInfo { //contains information required to correctly format if/while loopd for [] and {}
    std::vector<std::vector<Token>> startTokens;
    std::vector<int> indents; //the indent of the if/while loop
    std::vector<std::string> code; //code to come after the if/while loop
};


class FileWriter {
public:
    FileWriter(std::string fileName, std::string lang);
    ~FileWriter();
    void writeText(std::string text, std::string mode = "");
    void fileSetup(std::vector<startTerminals> theCollections);
    bool getCollectStartTerminalsFlag();
    void addStartTerminal(Token token);
private:
    std::string formatString(std::string base, std::string text);
    std::string indentString();
    std::string createStatement();
    void appendCode(std::string);
    SubsequentCodeInfo subCodeInfo;
    int indent; //defines how many spaces to use as the current indentation level
    std::string fileName;
    std::map<std::string, std::string> *language;
};

#endif