#ifndef GPARSER_HPP
#define GPARSER_HPP

#include <string>
#include "tokeniser.hpp"
#include "fileWriter.hpp"

//Grammar Parser

//parse functions
Gtoken parseGrammar(Tokeniser &tokeniser, FileWriter writer);
Gtoken grammar();
Gtoken rule();
Gtoken expression();
Gtoken term();
Gtoken factor();

#endif