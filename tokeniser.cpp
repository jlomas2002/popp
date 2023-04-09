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


Tokeniser::Tokeniser(string grammar, string tokensInput){
  Tokeniser::grammar.append(grammar);
  Tokeniser::index = 0; //Current position in grammar string
  Tokeniser::lineNum = 1;
  Tokeniser::pos = 1; //Current position in a rule - used for error feedback
  
  secondPass = false;
  tokenFileExists = false;

  if (tokensInput != ""){
    extractRegexes(tokensInput);
    tokenFileExists = true;
  }


  //This serves as a first pass to collect necessary information about the grammar
  firstPass();
  secondPass = true;
}


Tokeniser::~Tokeniser(){
  ;
}

vector<NonTerminalInfo> Tokeniser::getAllNonTerminalInfo(){
  return allNonTerminalInfo;
}

vector<TokenRegex> Tokeniser::getTokenRegexes(){
  return tokenRegexes;
}

set<string> Tokeniser::getListOfTokens(){
  return listOfTokens;
}

void Tokeniser::firstPass(){
  collectNonTerminalInfo(); 
  collectListOfNonTerminals();

  for (auto &ntInfo : allNonTerminalInfo){
    refineNonTerminalInfo(ntInfo, ntInfo);
  }

    //Remove any tokens in starting terminals list
  for (auto &ntInfo : allNonTerminalInfo){
    for (auto element = ntInfo.startingTerminals.begin(); element != ntInfo.startingTerminals.end(); ){
      if ((*element).type == Token_Type || (*element).type == Non_Terminal){
        element = ntInfo.startingTerminals.erase(element);
      }
      else{
        element++;
      }
    }
  }
}


//For a given token file, this extracts the regexes that describe the tokens
void Tokeniser::extractRegexes(string input){
  int index = 0;

  while(true){
    string id;

    if (index >= int(input.length())){
      return;
    }

    //Skip over any white space
    while (isspace(input.at(index))){
      index++;
      if (index >= int(input.length())) return;
    }

    //First extract the name of the token, can begin with a letter or _ char
    if (input.at(index) == '_' || isalpha(input.at(index))){
      id += input.at(index);
      index++;
      if (index >= int(input.length())) return;
    }
    //Add the rest of the name while a space char isnt seen
    while (!isspace(input.at(index))){
      id += input.at(index);
      index++;
      if (index >= int(input.length())) return;
    }

    //Skip over white space until the = char is seen
    while (isspace(input.at(index))){
      index++;
      if (index >= int(input.length())) return;
    }

    if (input.at(index) == '='){
      index++;
    }

    //Skip over white space until regex is reached
    while (isspace(input.at(index))){
      index++;
      if (index >= int(input.length())) return;
    }

    //Extract the regex
    string regex;
    while (!(input.at(index) == ';')){ //Add to regex until ; followed by a space is seen
      regex += input.at(index);
      index++;
      if (index >= int(input.length())) return;
    }


    //Add the regex struct to a list of structs
    TokenRegex tr;
    tr.name = id;
    tr.regex = regex;
    tokenRegexes.push_back(tr);

    if (listOfTokens.count(id)){
      //ERROR - Multiple definitions of token
    }
    else{
      listOfTokens.insert(id);
    }

  }
  return;
}

int Tokeniser::incrementIndex(){
  index++;
  pos++;
  if (index >= int(grammar.length())){
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

Error Tokeniser::checkTokOrNtError(std::string id){
  if (tokenFileExists){
    if (listOfNonTerminals.count(id) + listOfTokens.count(id) > 1){
      return RedefinedElement;
    }
    else if (listOfNonTerminals.count(id) + listOfTokens.count(id) < 1){
      return UndefinedElement;
    }
  }

  else{
    if (listOfNonTerminals.count(id) > 1){
      return RedefinedNonTerminal;
    }
  }

  return NONE;
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
  if (index >= int(grammar.length())){
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
    if (index >= int(grammar.length())){
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

  //Token is a non terminal or a token type
  else if(isalpha(grammar.at(index)) || grammar.at(index) == '_'){
    string lexeme;
    lexeme += grammar.at(index);
    if (incrementIndex() != 0) return makeErrorToken(UnexpectedEOF);
    
    while (isalpha(grammar.at(index)) || isdigit(grammar.at(index)) || grammar.at(index) == '_'){
      lexeme += grammar.at(index);
      if (incrementIndex() != 0) return makeErrorToken(UnexpectedEOF);
    }

    if (secondPass){
      Error err = checkTokOrNtError(lexeme);

      if (err == NONE){
        Gtoken token;
        token.lexeme = lexeme;
        token.lineNum = lineNum;
        token.pos = pos;
        token.error = NONE;

        if (listOfNonTerminals.count(lexeme) == 1){
          token.type = Non_Terminal;
        }

        else{
          token.type = Token_Type;
        }

        return token;
      }

      else{
        return makeErrorToken(err);
      }

    }

    else{
      Gtoken token;
      token.type = Non_Terminal;
      token.lexeme = lexeme;
      token.lineNum = lineNum;
      token.pos = pos;
      token.error = NONE;

      return token;
    }
  }

  //Token is a terminal
  else if(grammar.at(index) == '\"'){
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

  else{
    return makeErrorToken(UnknownCharacter);
  }
}

void Tokeniser::collectListOfNonTerminals(){
  for(auto ntInfo : allNonTerminalInfo){
    listOfNonTerminals.insert(ntInfo.nonTerminal.lexeme);
  }
}


//For each non terminal, this function calculates and stores all possible terminals that can begin it
//It Assumes for now grammar is syntactically valid, any syntactic errors are picked up by the parser
void Tokeniser::collectNonTerminalInfo(){
  Gtoken token;

  token = getNextToken();
  while (token.type != END_OF_GRAMMAR){
    NonTerminalInfo ntInfo;

    ntInfo.nonTerminal.lexeme = token.lexeme;

    token = getNextToken(); //Should consume the = symbol

    bool collectNext; //Flag that determines if the next terminal can start the non terminal

    int currentLevel = 0;
    int firstLevel = -1;  //The level where the first terminal/nonterminal was seen

    bool firstFoundInBracket = false;
    bool checkOutsideBracket = false;


    while (token.type != Symbol || token.lexeme != ";"){
      if (token.type == Symbol && (token.lexeme == "{" || token.lexeme == "[" || token.lexeme == "(")){
        currentLevel++;
      }
      if (token.type == Symbol && (token.lexeme == "}" || token.lexeme == "]" || token.lexeme == ")")){
        currentLevel--;
        if (firstFoundInBracket){
          if (currentLevel < firstLevel){
            //Bracket where first was found has closed
            checkOutsideBracket = true;
          }
        }
      }
      if (token.type == Symbol && (token.lexeme == "=" || token.lexeme == "|")){
        if (checkOutsideBracket){
          if (currentLevel < firstLevel){
            collectNext = true;
          }
        }
        else{
          if (firstLevel == -1 || currentLevel <= firstLevel){
            collectNext = true; //Collect the first instance of terminal/nonterminal after an = or |
          }
        }
      }
      if (collectNext && (token.type == Terminal || token.type == Non_Terminal)){
        if (firstLevel == -1){
          firstLevel = currentLevel;
          if (currentLevel > 0){
            firstFoundInBracket = true;
          }
        }
        ntInfo.startingTerminals.insert(token);
        collectNext = false;
      }
      token = getNextToken();
    }
    allNonTerminalInfo.push_back(ntInfo);
    token = getNextToken();
    
  }
  index = 0; //Return to start of grammar
}

NonTerminalInfo Tokeniser::findNonTerminalInfo(Gtoken t){
  for (auto ntInfo : allNonTerminalInfo){
    if (ntInfo.nonTerminal.lexeme == t.lexeme){
      return ntInfo;
    }
  }
}

//The current vector of terminals may contain non terminals
//This function will go through this vector, and convert any non terminals to the terminals or tokens that can begin it
void Tokeniser::refineNonTerminalInfo(NonTerminalInfo &returnNtInfo, NonTerminalInfo &currentNtInfo){
  auto termPointer = currentNtInfo.startingTerminals.begin(); //Iterator pointing to set elements

  while(termPointer != currentNtInfo.startingTerminals.end()){
    Gtoken t = *termPointer;

    if (t.type == Non_Terminal || t.type == Token_Type){

      if (t.type == Token_Type || listOfNonTerminals.count(t.lexeme) == 0){ //Assume it is a token
        t.type = Token_Type;

        if (returnNtInfo.nonTerminal.lexeme != currentNtInfo.nonTerminal.lexeme){ //Then need to add token to return non terminal info
          returnNtInfo.startingTokens.insert(t);
          returnNtInfo.startingTerminals.insert(t);
        }
        currentNtInfo.startingTokens.insert(t);

        if (!tokenFileExists){
          listOfTokens.insert(t.lexeme);
        }

        termPointer++;
      }

      else{ //It is a non terminal
        NonTerminalInfo nextNtInfo = findNonTerminalInfo(t);
    
        refineNonTerminalInfo(currentNtInfo, nextNtInfo);
        currentNtInfo.startingTerminals.erase(t);
        //Returned to original non terminal info, start from beginning as items in set not to the end
        termPointer = currentNtInfo.startingTerminals.begin();
      }
    }
    else{ //If terminal
      if (returnNtInfo.nonTerminal.lexeme != currentNtInfo.nonTerminal.lexeme){ //Then need to add terminal to return non terminal info
        returnNtInfo.startingTerminals.insert(t);
      }
      termPointer++;
    }
  }
}
