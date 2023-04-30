#ifndef GPARSER_HPP
#define GPARSER_HPP

#include <string>
#include "tokeniser.hpp"
#include "fileWriter.hpp"

//These functions perform parsing of the grammar.
//They form a recursive descent parser.
Gtoken parseGrammar(Tokeniser &tokeniser, FileWriter &writer);
Gtoken grammar();
Gtoken rule();
Gtoken expression();
Gtoken term();
Gtoken factor();

#endif
