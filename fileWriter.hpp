#ifndef WRITER_HPP
#define WRITER_HPP
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "tokenDef.hpp"

enum Mode {
    Func_Begin,
    Func_End,
    Terminal_Seen,
    NonTerminal_Seen,
    Get_NextTok,
    Peek_NextTok,
    If_Begin_Terminal,
    If_Begin_NonTerminal,
    ElseIf_Terminal,
    ElseIf_NonTerminal,
    If_End,
    Bracket_Begin,
    CurlyBracket_End,
    SquareBracket_End,
};


struct SubsequentCodeInfo { //contains information required to correctly format if/while loopd for [] and {}
    std::vector<std::vector<Gtoken>> startTokens;
    std::vector<int> indents; //the indent of the if/while loop
    std::vector<std::string> code; //code to come after the if/while loop
};


class FileWriter {
public:
    FileWriter(std::string fileName, std::string lang);
    ~FileWriter();
    void writeText(std::string text, Mode mode);
    void createLexer(bool makeTemplate, std::vector<TokenRegex> tokenRegexes);
    void fileSetup(std::vector<StartTermCollection> theCollections);
    bool getCollectStartTerminalsFlag();
    void addStartTerminal(Gtoken token);
private:
    std::string formatString(std::string base, std::string text);
    std::string indentString();
    std::string createConditionStatement();
    void appendCode(std::string);
    SubsequentCodeInfo subCodeInfo;
    int indent; //defines how many spaces to use as the current indentation level
    std::string fileName;
    std::map<std::string, std::string> *language;
};

#endif