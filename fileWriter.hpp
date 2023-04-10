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
    NonTerminal_Seen,
    Get_NextTok,
    Peek_NextTok,
    If_Begin_Terminal,
    If_Begin_NonTerminal,
    If_Begin_Token,
    ElseIf_Terminal,
    ElseIf_NonTerminal,
    ElseIf_Token,
    Scope_End,
    Else_Expr,
    Else_Fact_Terminal,
    Else_Fact_Token,
    Bracket_Begin,
    CurlyBracket_End,
    SquareBracket_End,
};

enum OutputStringType {
    functionHeader,
    tokDeclaration,
    getNextToken,
    peekNextToken,
    scopeEnd,
    callNonTerminal,
    ifHeader_terminal,
    ifHeader_nonTerminal,
    ifHeader_token,
    elseIfHeader_terminal,
    elseIfHeader_nonTerminal,
    elseIfHeader_token,
    elseHeader,
    whileHeader_begin,
    ifHeader_begin,
    defineStartTerminals_begin,
    defineStartTokens_begin,
    defineStartTerminals_list,
    defineStartTokens_list,
    defineStartTerminals_end,
    defineStartTokens_end,
    statement_terminalCheck,
    statement_tokenCheck,
    statement_nonTerminalCheck_terminal,
    statement_nonTerminalCheck_token,
    statement_terminalCheck_last,
    statement_tokenCheck_last,
    statement_nonTerminalCheck_terminal_last,
    statement_nonTerminalCheck_token_last,
    errorComment,

    lexer_initFunction,
    lexer_peekNextFunction,
    lexer_getNextFunction_header,
    lexer_includeRegex,
    lexer_regexDefintion,
    lexer_matchDeclaration,
    lexer_searchCode,
    lexer_headerFile,

    typeEnumDefinition_begin,
    typeEnumDefinition_end,
    tokenDefinition,
};


struct SubsequentCodeInfo { //contains information required to correctly format if/while loops for [] and {}
    std::vector<std::vector<Gtoken>> startTokens;
    std::vector<std::string> code; //code to come after the if/while loop
};


class FileWriter {
public:
    FileWriter(std::string fileName, std::string lang);
    ~FileWriter();
    void writeText(std::string text, Mode mode);
    void fileSetup(std::vector<NonTerminalInfo> allNtInfo, std::set<std::string> allTokens, std::string tokenFile, std::vector<TokenRegex> tokenRegexes);
    void addStartTerminal(Gtoken token);
private:
    NonTerminalInfo getNtInfo(std::string);
    void createLexer(bool makeTemplate, std::vector<TokenRegex> tokenRegexes);
    void createTokenDef(std::set<std::string> allTokenTypes);
    std::string formatString(std::string base, std::string text);
    std::string indentString();
    std::string createConditionStatement();
    std::string createElseCode();
    void appendCode(std::string);
    void writeFuncToParserFile();
    void addNewSubCode();

    std::vector<NonTerminalInfo> allNonTerminalInfo;
    SubsequentCodeInfo subCodeInfo;
    int indent; //defines how many spaces to use as the current indentation level
    std::string fileName;
    std::map<OutputStringType, std::string> *language;
};

#endif