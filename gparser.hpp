#ifndef GPARSER_HPP
#define GPARSER_HPP

#include <string>
#include "tokeniser.hpp"
#include "fileWriter.hpp"

//Grammar Parser

//parse functions
void parseGrammar(Tokeniser &tokeniser, FileWriter writer);
void grammar();
void rule();
void expression();
void term();
void factor();

#endif