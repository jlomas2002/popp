#include <iostream>
#include "tokeniser.hpp"
#include "gparser.hpp"
using namespace std;

//todo
//implement tokeniser
void Tokeniser::setGrammar(std::string grammar){
  Tokeniser::grammar.append(grammar);
  Tokeniser::index = 0;
}

void Tokeniser::tokenise(){
  parseGrammar(Tokeniser::grammarInfo);
}

Token Tokeniser::peekNextToken(){
  grammar.at(index);
}

Token Tokeniser::getNextToken(){

}