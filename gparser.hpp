#ifndef GPARSER_HPP
#define GPARSER_HPP

#include <string>
#include "tokeniser.hpp"

//Grammar Parser

//define elements

//parse functions
void parseGrammar(GrammarInfo &grammarInfo);
void grammar();
void rule();
void rhs();
void lhs();
void terminal();
void nonTerminal();
void character();
void symbol();
void digit();
void letter();

#endif