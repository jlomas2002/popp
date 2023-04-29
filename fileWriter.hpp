#ifndef WRITER_HPP
#define WRITER_HPP
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
    TerminalCheck_End,
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
    addActionComment,
    pythonPass,
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
    defineFirstTerminals_begin,
    defineFirstTokens_begin,
    defineFirstTerminals_list,
    defineFirstTokens_list,
    defineFirstTerminals_end,
    defineFirstTokens_end,
    listEnd,
    statement_terminalCheck,
    statement_tokenCheck,
    statement_nonTerminalCheck_terminal,
    statement_nonTerminalCheck_token,
    statement_terminalCheck_last,
    statement_tokenCheck_last,
    statement_nonTerminalCheck_terminal_last,
    statement_nonTerminalCheck_token_last,
    errorString_begin,
    errorString_append,
    errorString_end,
    errorOutput,
    exitWithError,
    parserIncludes,
    parserMain,

    lexer_initFunction,
    lexer_peekNextFunction,
    lexer_skipWhitespace,
    lexer_includes,
    lexer_regexDefintion,
    lexer_matchDeclaration,
    lexer_searchCode,
    lexer_returnNextChar,
    lexer_headerFile,

    typeEnumDefinition_begin,
    typeEnumDefinition_end,
    tokenDefinition,
    tokenDefinitionIncludes,
};


struct SubsequentCodeInfo { //contains information required to correctly format if/while loops for [] and {}
    std::vector<std::vector<Gtoken>> startTokens;
    std::vector<std::string> code; //code to come after the if/while loop
};


class FileWriter {
public:
    FileWriter(std::string lang);
    ~FileWriter();
    void writeText(std::string text, Mode mode);
    void fileSetup(std::vector<FirstSetInfo> allFirstSetInfo, std::set<std::string> allTokens, std::vector<TokenRegex> tokenRegexes, std::string projectName);
    void addStartTerminal(Gtoken token);
    void createMain(std::string startFunc);
    std::string getParserText();
    std::string getLexerText();
    std::string getTokDefText();
    std::string getParserHeaderText();
    std::string getLexerHeaderText();
private:
    FirstSetInfo getFirstSetInfo(std::string);
    void createLexer(std::vector<TokenRegex> tokenRegexes);
    void createTokenDef(std::set<std::string> allTokenTypes);
    void createParserHeader();
    std::string formatString(std::string base, std::string text);
    std::string indentString();
    std::string createConditionStatement();
    std::string createElseCode();
    void appendCode(std::string);
    void writeFuncToParserFile();
    void addNewSubCode();

    std::vector<FirstSetInfo> allFirstSetInfo;
    SubsequentCodeInfo subCodeInfo;
    int indent; //defines how many spaces to use as the current indentation level
    std::map<OutputStringType, std::string> *language;
    std::string projectName;
    std::string parserText;
    std::string lexerText;
    std::string tokDefText;
    std::string parserHeaderText;
    std::string lexerHeaderText;
};

#endif
