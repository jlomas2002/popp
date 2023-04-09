#ifndef TOKENDEF_HPP
#define TOKENDEF_HPP

#include <string>
#include <set>

enum Type {
  Terminal,
  Non_Terminal,
  Symbol,
  Token_Type,

  ERROR,
  END_OF_GRAMMAR,
};

enum Error {
  UnexpectedEOF, //Grammar string suddenly ends when it shouldn't
  UnknownCharacter,
  NonTerminalNoDefintion,
  RedefinedElement,
  RedefinedNonTerminal,
  UndefinedElement,

  ExpectedNonTerminal,
  ExpectedEquals,
  ExpectedSemiColon,
  ExpectedCurlyBracket,
  ExpectedNormalBracket,
  ExpectedSquareBracket,
  SyntacticError,

  NONE //Default of no error
};
 
//  std::string typeToString(Type t){
//   switch(t)
//   {
//       case Terminal  : std::cout << "Terminal\n";   break;
//       case Non_Terminal  : std::cout << "Non Terminal\n";   break;
//       case Symbol  : std::cout << "Symbol\n";   break;
//       case Token_Type  : std::cout << "Token\n";   break;
//       case ERROR  : std::cout << "Error\n";   break;
//       case END_OF_GRAMMAR  : std::cout << "End Of Grammar\n";   break;
//   }
// }

//Tokens in a ebnf grammar file
struct Gtoken {
  Type type;
  std::string lexeme;
  int lineNum;
  int pos; //Position on line
  std::string error;
};


//Stores the name of a non terminal, and a set of all of the terminals that can start it
struct NonTerminalInfo{
  Gtoken nonTerminal;
  std::set<Gtoken> startingTerminals;
  std::set<Gtoken> startingTokens;
};

//Stores the name of a token and its corresponding regex
struct TokenRegex {
  std::string name;
  std::string regex;
};

#endif