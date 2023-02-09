#include <iostream>
#include <cctype>
#include "tokeniser.hpp"
#include "gparser.hpp"
using namespace std;

std::set<char> keySymbols = {'(', ')', '{', '}', '[', ']', ',', '|', '=', ';'};

//todo
//implement tokeniser
void Tokeniser::setGrammar(std::string grammar){
  Tokeniser::grammar.append(grammar);
  Tokeniser::index = 0;
  Tokeniser::lineNum = 1;
}

void Tokeniser::tokenise(){
  //parseGrammar(Tokeniser::grammarInfo);
}

Token Tokeniser::peekNextToken(){
  int tmp = index;
  Token token;
  token = getNextToken();
  index = tmp;
  return token; 
}

Token Tokeniser::getNextToken(){
  //skip over white space
  if (isspace(grammar.at(index))){
    while (isspace(grammar.at(index))){
      index++;
    }
  }

  //terminal
  if(grammar.at(index) == '"'){
    string lexeme;
    index++;
    while (grammar.at(index) != '"'){
      lexeme += grammar.at(index);
      index++;
    }
    Token token;
    token.lexeme = lexeme;
    token.type = "terminal";
    token.lineNum = lineNum;
    index++;
    return token;
  }

  //reserved symbol
  if(keySymbols.count(grammar.at(index))){
    Token token;
    token.lexeme = grammar.at(index);
    token.type = "symbol";
    token.lineNum = lineNum;
    if (grammar.at(index) == ';')
      lineNum++;
    index++;
    return token;
  }

  //non terminal
  if(isalpha(grammar.at(index))){
    string lexeme;
    lexeme += grammar.at(index);
    index++;
    while (isalpha(grammar.at(index)) || isdigit(grammar.at(index)) || grammar.at(index) == '_'){
      lexeme += grammar.at(index);
      index++;
    }

    Token token;
    token.lexeme = lexeme;
    token.type = "nonTerminal";
    token.lineNum = lineNum;
    return token;
  }
  
  if (grammar.at(index) == EOF){
    Token token;
    token.lexeme = "EOF";
    token.type = "endOfFile";
    token.lineNum = lineNum;
    return token;
  }
}