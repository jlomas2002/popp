#ifndef GPARSER_HPP
#define GPARSER_HPP

#include <string>
#include "tokeniser.hpp"

//Grammar Parser

//define elements

//parse functions
void parseGrammar(Tokeniser &tokeniser);
void grammar();
void rule();
void expression();
void term();
void factor();

#endif