#include <iostream>
#include <cctype>
#include "tokeniser.hpp"
#include "gparser.hpp"
using namespace std;

//These have special meaning in a grammar file
std::set<char> reservedSymbols = {'(', ')', '{', '}', '[', ']', '|', '=', ';'};


//So tokens in a set can be placed in an order
bool operator<(const Gtoken &a, const Gtoken &b){
    return a.lexeme < b.lexeme;
}


Tokeniser::Tokeniser(string grammar){
  Tokeniser::grammar.append(grammar);
  Tokeniser::index = 0; //Current position in grammar string
  Tokeniser::lineNum = 1;
  Tokeniser::pos = 1; //Current position in a rule - used for error feedback

  collectStartTerms(); //Store information about the terminals that can begin a non terminal in the startTermCollections data struct.
}


//For a given token file, this extracts the regexes that describe the tokens
vector<TokenRegex> Tokeniser::extractRegexes(string input){
  vector<TokenRegex> regexes;
  int index = 0;

  while(true){
    string id;

    //First extract the name of the token, can begin with a letter or _ char
    if (input.at(index) == '_' || isalpha(input.at(index))){
      id += input.at(index);
      index++;
      if (index >= input.length()) return regexes;
    }
    //Add the rest of the name while a space char isnt seen
    while (!isspace(input.at(index))){
      id += input.at(index);
      index++;
      if (index >= input.length()) return regexes;
    }

    //Skip over white space until the = char is seen
    while (isspace(input.at(index))){
      index++;
      if (index >= input.length()) return regexes;
    }

    if (input.at(index) == '='){
      index++;
    }

    //Skip over white space until regex is reached
    while (isspace(input.at(index))){
      index++;
      if (index >= input.length()) return regexes;
    }

    //Extract the regex
    string regex;
    while (!(input.at(index) == ';' && isspace(input.at(index + 1)))){ //Add to regex until ; followed by a space is seen
      regex += input.at(index);
      index++;
      if (index >= input.length()) return regexes;
    }

    //Add the regex struct to a list of structs
    TokenRegex tr;
    tr.name = id;
    tr.regex = regex;
    regexes.push_back(tr);

    //Skip over remaining white space
    while (isspace(input.at(index))){
      index++;
      if (index >= input.length()) return regexes;
    }
  }
  
  return regexes;
}

Tokeniser::~Tokeniser(){
  ;
}

vector<StartTermCollection> Tokeniser::getStartTermCollections(){
  return startTermCollections;
}

int Tokeniser::incrementIndex(){
  index++;
  pos++;
  if (index >= grammar.length()){
    return -1;
  }

  return 0;
}

Gtoken Tokeniser::makeErrorToken(Error err){
  Gtoken token;

  token.type = ERROR;
  token.lexeme = "";
  token.lineNum = lineNum;
  token.pos = pos;
  token.error = err;

  return token;
}

//Returns the next token in the grammar file, but doesn't consume it
Gtoken Tokeniser::peekNextToken(){
  int tmp = index;
  Gtoken token;
  token = getNextToken();
  index = tmp;
  return token; 
}

//Returns the next token in the grammar file, and consumes it
Gtoken Tokeniser::getNextToken(){

  //Check if reached end of grammar
  if (index >= grammar.length()){
    Gtoken token;

    token.type = END_OF_GRAMMAR;
    token.lexeme = "";
    token.lineNum = lineNum;
    token.pos = pos;
    token.error = NONE;

    return token;
  }

  //Skip over white space
  while (isspace(grammar.at(index))){

    if (grammar.at(index) == '\n'){
      lineNum++;
      pos = 1;
    }

    incrementIndex();
    if (index >= grammar.length()){
      Gtoken token;

      token.type = END_OF_GRAMMAR;
      token.lexeme = "";
      token.lineNum = lineNum;
      token.pos = pos;
      token.error = NONE;

      return token;
    }
  }

  //Token is reserved symbol
  if(reservedSymbols.count(grammar.at(index))){
    Gtoken token;

    token.type = Symbol;
    token.lexeme = grammar.at(index);
    token.lineNum = lineNum;
    token.pos = pos;
    token.error = NONE;

    incrementIndex();

    return token;
  }

  //Token is a non terminal
  if(isalpha(grammar.at(index) || grammar.at(index) == '_')){
    string lexeme;
    lexeme += grammar.at(index);
    if (incrementIndex() != 0) return makeErrorToken(UnexpectedEOF);
    
    while (isalpha(grammar.at(index)) || isdigit(grammar.at(index)) || grammar.at(index) == '_'){
      lexeme += grammar.at(index);
      if (incrementIndex() != 0) return makeErrorToken(UnexpectedEOF);
    }

    Gtoken token;
    token.type = Non_Terminal;
    token.lexeme = lexeme;
    token.lineNum = lineNum;
    token.pos = pos;
    token.error = NONE;

    incrementIndex();
    return token;
  }

  //Token is a terminal
  if(grammar.at(index) == '\"'){
    string lexeme;
    if (incrementIndex() != 0) return makeErrorToken(UnexpectedEOF);

    while (grammar.at(index) != '\"'){
      lexeme += grammar.at(index);
      if (incrementIndex() != 0) return makeErrorToken(UnexpectedEOF);
    }

    Gtoken token;

    token.type = Terminal;
    token.lexeme = lexeme;
    token.lineNum = lineNum;
    token.pos = pos;
    token.error = NONE;

    incrementIndex();
    return token;
  }

}

//For each non terminal, this function calculates and stores all possible terminals that can begin it
//It Assumes for now grammar is syntactically valid, any syntactic errors are picked up by the parser
void Tokeniser::collectStartTerms(){
  Gtoken token;
  token = getNextToken();

  while (token.type != END_OF_GRAMMAR){
    StartTermCollection collection;

    collection.nonTerminal.lexeme = token.lexeme;

    bool collectNext; //Flag that determines if the next terminal can start the non terminal
    token = getNextToken(); //Should consume the = symbol
    int currentLevel = 0;
    int firstLevel = -1;  //The level where the first terminal/nonterminal was seen

    while (token.type != Symbol || token.lexeme != ";"){
      if (token.type == Symbol && (token.lexeme == "{" || token.lexeme == "[" || token.lexeme == "(")){
        currentLevel++;
      }
      if (token.type == Symbol && (token.lexeme == "}" || token.lexeme == "]" || token.lexeme == "(")){
        currentLevel--;
      }
      if (token.type == Symbol && (token.lexeme == "=" || token.lexeme == "|")){
        if (firstLevel == -1 || currentLevel <= firstLevel){
          collectNext = true;                   //Collect the first instance of terminal/nonterminal after an = or |
        }
      }
      if (collectNext && (token.type == Terminal || token.type == Non_Terminal)){
        if (firstLevel == -1){
          firstLevel = currentLevel;
        }
        collection.terminals.insert(token);
        collectNext = false;
      }
      token = getNextToken();
    }
    startTermCollections.push_back(collection);
    token = getNextToken();
  }
  index = 0; //Return to start of grammar

  for (auto &collection : startTermCollections){
    refineStartTerms(collection, collection);
  }
}

StartTermCollection Tokeniser::matchNonTerminalToCollection(Gtoken t){
  for (auto collection : startTermCollections){
    if (collection.nonTerminal.lexeme == t.lexeme){
      return collection;
    }
  }
}

//The current vector of terminals may contain non terminals
//This function will go through this vector, and convert any non terminals to the terminals that can begin it
void Tokeniser::refineStartTerms(StartTermCollection &returnCollection, StartTermCollection &currentCollection){
  auto termPointer = currentCollection.terminals.begin(); //Iterator pointing to set elements

  while(termPointer != currentCollection.terminals.end()){
    Gtoken t = *termPointer;

    if (t.type == Non_Terminal){
      StartTermCollection nextCollection = matchNonTerminalToCollection(t);
  
      refineStartTerms(currentCollection, nextCollection);
      currentCollection.terminals.erase(t);
      //Returned to original collection, start from beginning as items in set not to the end
      termPointer = currentCollection.terminals.begin();
    }
    else{ //If terminal
      if (returnCollection.nonTerminal.lexeme != currentCollection.nonTerminal.lexeme){ //Then need to add terminal to return collection
        returnCollection.terminals.insert(t);
      }
      termPointer++;
    }
  }
}
