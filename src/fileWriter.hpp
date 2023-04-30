#ifndef WRITER_HPP
#define WRITER_HPP
#include <string>
#include <vector>
#include <map>
#include "tokenDef.hpp"

/**
 * @brief Enum representing each code pattern to output to the file
 * 
 */
enum Mode {
    Func_Begin,
    Func_End,
    NonTerminal_Seen,
    Get_NextTok,
    Get_NextTok_Commented,
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

/**
 * @brief Enum representing different strings of code in each language.
 * 
 */
enum OutputStringType {
    functionHeader,
    tokDeclaration,
    getNextToken,
    getNextToken_commented,
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
    errorComment,
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


/**
 * @brief Stores a new block of code for each expression
 * A new expression is created at the start of a rule or when an opeining bracket is seen.
 * Each block stores the resultant code (the while/if loop onwards) and the elements that can start the expression.
 * 
 */
struct ExpressionBlockInfo { 
    std::vector<std::vector<Gtoken>> firstSet;
    std::vector<std::string> code; 
};

/**
 * @brief Handles all of the output to generated files.
 * 
 */
class FileWriter {
public:
    /**
     * @brief Constructor - Sets the pointer to the target language specified by lang.
     * 
     * @param lang The language output files are written in.
     */
    FileWriter(std::string lang);
    ~FileWriter();
    /**
     * @brief Writes text to parser file depending on the mode.
     * 
     * @param text Normally some variable string used in the output code.
     * @param mode Determines the pattern of code generated.
     */
    void writeText(std::string text, Mode mode);
    /**
     * @brief Generates start of parser and any other files, e.g lexer.
     * 
     * @param allFirstSetInfo List of each non terminal and its first set
     * @param allTokens Set of token names
     * @param tokenRegexes Vector of tokens and their correspondin regexes
     * @param projectName Name of project
     */
    void fileSetup(std::vector<FirstSetInfo> allFirstSetInfo, std::set<std::string> allTokens, std::vector<TokenRegex> tokenRegexes, std::string projectName);
    /**
     * @brief Adds a Gtoken to the first set of the current expression.
     * 
     * @param token The token to be added.
     */
    void addFirstElement(Gtoken token);
    /**
     * @brief Creates the main function to be included in the parser.
     * 
     * @param startFunc The name of the function to be called in main.
     */
    void createMain(std::string startFunc);
    /**
     * @brief Returns the code of the output parser.
     * 
     * @return std::string 
     */
    std::string getParserText();
    /**
     * @brief Returns the code of the output lexer.
     * 
     * @return std::string 
     */
    std::string getLexerText();
    /**
     * @brief Returns the code of the output token definition.
     * 
     * @return std::string 
     */
    std::string getTokDefText();
    /**
     * @brief Returns the code of the output parser header.
     * 
     * @return std::string 
     */
    std::string getParserHeaderText();
    /**
     * @brief Returns the code of the output lexer header.
     * 
     * @return std::string 
     */
    std::string getLexerHeaderText();
private:
    FirstSetInfo getFirstSetInfo(std::string);
    void createLexer(std::vector<TokenRegex> tokenRegexes);
    void createTokenDef(std::set<std::string> allTokenTypes);
    void createParserHeader();

    //Replaces {0} in base string with the text string.
    std::string formatString(std::string base, std::string text);
    //Returns a string of spaces with length specified by index.
    std::string indentString();
    //Creates the body of if statement or while loop when an opening bracket seen in grammar.
    std::string createConditionStatement();
    std::string createElseCode();
    //Adds code of the current expression to the previous expression.
    void appendCode(std::string);
    void writeFuncToParserFile();
    void addNewExprBlock();

    std::vector<FirstSetInfo> allFirstSetInfo;
    ExpressionBlockInfo exprBlockInfo;
    int indent; //Defines how many spaces to use as the current indentation level
    std::map<OutputStringType, std::string> *language;

    //Stores the name of the project - used to name output files.
    std::string projectName;

    //Strings to store output code.
    std::string parserText;
    std::string lexerText;
    std::string tokDefText;
    std::string parserHeaderText;
    std::string lexerHeaderText;
};

#endif
