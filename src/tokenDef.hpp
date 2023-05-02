#ifndef TOKENDEF_HPP
#define TOKENDEF_HPP

#include <string>
#include <set>

/**
 * @brief Represents the different types of token in a grammar file.
 * 
 */
enum Type {
    Terminal,
    Non_Terminal,
    Symbol,
    Token_Type,
    Error_Type,
    END_OF_GRAMMAR,
};

/**
 * @brief Represents the different types of error that can be seen in a grammar or token file.
 * 
 */
enum Error {
    //Dealt with by tokeniser
    RedefinedElement,
    RedefinedNonTerminal,
    RedefinedToken,

    //Dealt with by parser
    UnexpectedEOF, //String suddenly ends when it shouldn't
    UnknownCharacter,
    UndefinedToken,
    ExpectedNonTerminal,
    ExpectedEquals,
    ExpectedSemiColon,
    ExpectedCurlyBracket,
    ExpectedNormalBracket,
    ExpectedSquareBracket,

    //Token file only errors
    InvalidRegexId,

    NONE //Default of no error
};
 
/**
 * @brief Stores information about a token
 * 
 */
struct Gtoken {
    Type type;
    std::string lexeme;
    int lineNum;
    int pos; //Position on line
    Error error;
};

/**
 * @brief Stores the first set of a non terminal, represented by a set of terminals and a set of tokens that can begin the non terminal.
 * 
 */
struct FirstSetInfo{
    Gtoken nonTerminal;
    std::set<Gtoken> firstTerminals;
    std::set<Gtoken> firstTokens;
};

/**
 * @brief Stores the name of a token and its corresponding regex.
 * 
 */
struct TokenRegex {
    std::string name;
    std::string regex;
};

#endif
