#include <iostream>
#include <cctype>
#include "tokeniser.hpp"
#include "gparser.hpp"
using namespace std;

std::set<char> keySymbols = {'(', ')', '{', '}', '[', ']', ',', '|', '=', ';'};

bool operator<(const Token &a, const Token &b){
    return a.lexeme < b.lexeme;
}


//todo
//implement tokeniser
Tokeniser::Tokeniser(std::string grammar){
  Tokeniser::grammar.append(grammar);
  Tokeniser::index = 0;
  Tokeniser::lineNum = 1;
  collectStartTerminals();
  refineStartTerminals();
  for (auto el: nonTerminalInfo){
    cout<<el.nonTerminal<<" #";
    for (auto t: el.terminals){
      cout<<t.lexeme<<" ";
    }
    cout<<"\n";
  }
}

Tokeniser::~Tokeniser(){
  ;
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
  //check if other tokens exist / reached end of grammar
  if (index >= grammar.length()){
    Token token;
    token.lexeme = "ENDOFGRAMMAR";
    return token;
  }

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
    cout<<"eof reached"<<endl;
    Token token;
    token.lexeme = "EOF";
    token.type = "endOfFile";
    token.lineNum = lineNum;
    return token;
  }
}

//for each non terminal, this function calculates and stores all possible terminals that can begin it
//assumes for now grammar is valid, any syntactic error are picked up by the parser
void Tokeniser::collectStartTerminals(){
  Token token;
  token = getNextToken();
  while (token.lexeme != "ENDOFGRAMMAR"){
    startTerminals st;
    if (token.type == "nonTerminal"){
      st.nonTerminal = token.lexeme;
    }
    bool collectNext;
    token = getNextToken();
    int currentLevel = 0;
    int firstLevel = -1;  //the level where the first terminal/nonterminal was seen. Also acts as flag
    while (token.type != "symbol" || token.lexeme != ";"){
      if (token.type == "symbol" && (token.lexeme == "{" || token.lexeme == "[")){
        currentLevel++;
      }
      if (token.type == "symbol" && (token.lexeme == "}" || token.lexeme == "]")){
        currentLevel--;
      }
      if (token.type == "symbol" && (token.lexeme == "=" || token.lexeme == "|")){
        if (firstLevel == -1 || currentLevel == firstLevel){
          collectNext = true;                   //collects first instance of terminal/nonterminal after an = or |
        }
      }
      if (collectNext && (token.type == "terminal" || token.type == "nonTerminal")){
        if (firstLevel == -1){
          firstLevel = currentLevel;
        }
        st.terminals.insert(token);
        collectNext = false;
      }
      token = getNextToken();
    }
    nonTerminalInfo.push_back(st);
    token = getNextToken();
  }
  index = 0;
}

void Tokeniser::insertTerminals(string nonTerminal, startTerminals &insertCollection){
  for (auto &collection : nonTerminalInfo){
    if (collection.nonTerminal == nonTerminal){
      for (auto t : collection.terminals){
        if (t.type == "terminal"){
          insertCollection.terminals.insert(t);
        }
        else{ //non terminal
          insertTerminals(t.lexeme, collection);
          collection.terminals.erase(t);
        }
      }
    }
  }
}

//The current vector of terminals may contain non terminals
//This function will go through this vector, and convert any non terminals to the terminals that can begin it
void Tokeniser::refineStartTerminals(){
  for (auto &collection : nonTerminalInfo){
    for (auto t : collection.terminals){
      if (t.type == "nonTerminal"){
        insertTerminals(t.lexeme, collection);
      }
    }
  }
}
