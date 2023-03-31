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

  ExpectedEquals,
  ExpectedSemiColon,
  ExpectedCurlyBracket,
  ExpectedNormalBracket,
  ExpectedSquareBracket,
  SyntacticError,

  NONE //Default of no error
};
 
// std::string typeToString(Type t){
//   switch(r)
//   {
//       case red  : std::cout << "red\n";   break;
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
struct StartTermCollection {
  Gtoken nonTerminal;
  std::set<Gtoken> terminals;
};

//Stores the name of a token and its corresponding regex
struct TokenRegex {
  std::string name;
  std::string regex;
};

#endif