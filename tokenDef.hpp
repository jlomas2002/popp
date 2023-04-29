#ifndef TOKENDEF_HPP
#define TOKENDEF_HPP

#include <string>
#include <set>

enum Type {
    Terminal,
    Non_Terminal,
    Symbol,
    Token_Type,
    Error_Type,
    END_OF_GRAMMAR,
};

enum Error {
    //Dealt with by tokeniser
    RedefinedElement,
    RedefinedNonTerminal,
    RedefinedToken,

    //Dealt with by parser
    UnexpectedEOF, //Grammar string suddenly ends when it shouldn't
    UnknownCharacter,
    UndefinedToken,
    ExpectedNonTerminal,
    ExpectedEquals,
    ExpectedSemiColon,
    ExpectedCurlyBracket,
    ExpectedNormalBracket,
    ExpectedSquareBracket,

    //Token only errors
    InvalidRegexId,

    NONE //Default of no error
};
 
//Tokens in a ebnf grammar file
struct Gtoken {
    Type type;
    std::string lexeme;
    int lineNum;
    int pos; //Position on line
    Error error;
};


//Stores the name of a non terminal, and a set of all of the terminals that can start it
struct FirstSetInfo{
    Gtoken nonTerminal;
    std::set<Gtoken> firstTerminals;
    std::set<Gtoken> firstTokens;
};

//Stores the name of a token and its corresponding regex
struct TokenRegex {
    std::string name;
    std::string regex;
};

#endif
