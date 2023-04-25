#include <iostream>
#include <cctype>
#include "tokeniser.hpp"
#include "gparser.hpp"
using namespace std;

//These have special meaning in a grammar file
set<char> reservedSymbols = {'(', ')', '{', '}', '[', ']', '|', '=', ';'};


//So tokens in a set can be placed in an order
bool operator<(const Gtoken &a, const Gtoken &b){
    return a.lexeme < b.lexeme;
}

bool operator==(const Gtoken &a, const Gtoken &b){
    return a.lexeme == b.lexeme;
}


Tokeniser::Tokeniser(string grammar, string tokensInput){
  Tokeniser::grammar.append(grammar);
  Tokeniser::index = 0; //Current position in grammar string
  Tokeniser::lineNum = 1;
  Tokeniser::pos = 1; //Current position in a rule - used for error feedback
  
  secondPass = false;
  tokenFileExists = false;
  erroneous = false;

  if (tokensInput != ""){
    Gtoken checkErrorTok = extractRegexes(tokensInput);
    if (checkErrorTok.type == ERROR){
      erroneous = true;
      tokenFileError = checkErrorTok;
    }
    tokenFileExists = true;
  }

  if (!erroneous){
    Gtoken tok;
    tok.error = NONE;
    tokenFileError = tok;
  }

  Tokeniser::index = 0;
  Tokeniser::lineNum = 1;
  Tokeniser::pos = 1;


  //This serves as a first pass to collect necessary information about the grammar
  firstPass();
  secondPass = true;

  Tokeniser::index = 0;
  Tokeniser::lineNum = 1;
  Tokeniser::pos = 1;
}


Tokeniser::~Tokeniser(){
  ;
}

vector<FirstSetInfo> Tokeniser::getAllFirstSetInfo(){
  return allFirstSetInfo;
}

vector<TokenRegex> Tokeniser::getTokenRegexes(){
  return tokenRegexes;
}

set<string> Tokeniser::getListOfTokens(){
  return listOfTokens;
}

bool Tokeniser::getErrorState(){
  return erroneous;
}

Gtoken Tokeniser::getTokenFileError(){
  return tokenFileError;
}

Gtoken Tokeniser::getGrammarFileError(){
  return grammarFileError;
}

void Tokeniser::firstPass(){
  collectFirstSetInfo(); 
  Gtoken checkErrorTok = collectListOfNonTerminals();

  if (checkErrorTok.type == ERROR){
    erroneous = true;
    grammarFileError = checkErrorTok;

    //File is erroneous, thus return
    return;
  }

  Gtoken tok;
  tok.error = NONE;
  grammarFileError = tok;

  for (auto &info : allFirstSetInfo){
    refineFirstSetInfo(info, info);
  }


  //Remove any tokens in starting terminals list
  for (auto &info : allFirstSetInfo){
    for (auto element = info.firstTerminals.begin(); element != info.firstTerminals.end(); ){
      if ((*element).type == Token_Type || (*element).type == Non_Terminal){
        element = info.firstTerminals.erase(element);
      }
      else{
        element++;
      }
    }
  }
}

Error Tokeniser::checkUndefinedError(std::string id){
  if (tokenFileExists){
    if (listOfNonTerminals.count(id) + listOfTokens.count(id) < 1){
      return UndefinedToken;
    }
  }
  return NONE;
}

Gtoken Tokeniser::collectListOfNonTerminals(){
  for(auto info : allFirstSetInfo){
    //Here we can check for redefinition of a non terminal
    if (tokenFileExists){
      if (listOfTokens.count(info.nonTerminal.lexeme)){
        lineNum = info.nonTerminal.lineNum;
        pos = info.nonTerminal.pos;
        return makeErrorToken(RedefinedElement, info.nonTerminal.lexeme);
      }
    }

    if (listOfNonTerminals.count(info.nonTerminal.lexeme)){
      lineNum = info.nonTerminal.lineNum;
      pos = info.nonTerminal.pos;
      return makeErrorToken(RedefinedNonTerminal, info.nonTerminal.lexeme);
    }

    listOfNonTerminals.insert(info.nonTerminal.lexeme);
  }

  Gtoken tok;
  tok.error = NONE;
  return tok;
}

int Tokeniser::incrementIndex(string input){
  index++;
  pos++;
  
  if (index >= int(input.length())){
    return -1;
  }

  return 0;
}

Gtoken Tokeniser::makeErrorToken(Error err, string lex){
  Gtoken token;

  token.type = ERROR;
  token.lexeme = lex;
  token.lineNum = lineNum;
  token.pos = pos;
  token.error = err;

  return token;
}

//For a given token file, this extracts the regexes that describe the tokens
Gtoken Tokeniser::extractRegexes(string input){
  index = 0;
  lineNum = 0;

  Gtoken token;

  while(true){
    string id;

    lineNum++;
    pos = 1;
    
    if (index >= int(input.length())){
      token.type = END_OF_GRAMMAR;
      token.lexeme = "";
      token.lineNum = lineNum;
      token.error = NONE;
      return token;
    }

    //Skip over any white space
    while (isspace(input.at(index))){
      if (incrementIndex(input) != 0){
        token.type = END_OF_GRAMMAR;
        token.lexeme = "";
        token.lineNum = lineNum;
        token.error = NONE;
        return token;
      }
    }

    //First extract the name of the token, can begin with a letter or _ char
    if (input.at(index) == '_' || isalpha(input.at(index))){
      int startPos = pos;
      id += input.at(index);
      if (incrementIndex(input) != 0) return makeErrorToken(UnexpectedEOF, "");
    }
    else{
      return makeErrorToken(InvalidRegexId, string(1, input.at(index)));
    }
    //Add the rest of the name while a space char isnt seen
    while (isalpha(input.at(index)) || isdigit(input.at(index))){
      id += input.at(index);
      if (incrementIndex(input) != 0) return makeErrorToken(UnexpectedEOF, "");
    }

    //Skip over white space until the = char is seen
    while (isspace(input.at(index))){
      if (incrementIndex(input) != 0) return makeErrorToken(UnexpectedEOF, "");
    }

    if (input.at(index) == '='){
      if (incrementIndex(input) != 0) return makeErrorToken(UnexpectedEOF, "");
    }
    else{
      return makeErrorToken(ExpectedEquals, string(1, input.at(index)));
    }

    //Skip over white space until regex is reached
    while (isspace(input.at(index))){
      if (incrementIndex(input) != 0) return makeErrorToken(UnexpectedEOF, "");
    }

    //Extract the regex
    string regex;
    while (!(input.at(index) == ';')){ //Add to regex until ; followed by a space is seen
      regex += input.at(index);
      if (incrementIndex(input) != 0) return makeErrorToken(UnexpectedEOF, "");
    }

    if (listOfTokens.count(id)){
      //ERROR - Multiple definitions of token
      return makeErrorToken(RedefinedToken, id);
    }

    //Add the regex struct to a list of structs
    TokenRegex tr;
    tr.name = id;
    tr.regex = regex;
    tokenRegexes.push_back(tr);

    listOfTokens.insert(id);

    if (incrementIndex(input) != 0){
      token.type = END_OF_GRAMMAR;
      token.lexeme = "";
      token.lineNum = lineNum;
      token.error = NONE;
      return token;
  }

  }
  Gtoken tok;
  tok.error = NONE;
  return tok;
}


//Returns the next token in the grammar file, but doesn't consume it
Gtoken Tokeniser::peekNextToken(){
  int tmpIndex = index;
  int tmpLinenum = lineNum;
  int tmpPos = pos;

  Gtoken token;
  token = getNextToken();

  index = tmpIndex;
  lineNum = tmpLinenum;
  pos = tmpPos;

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
    if (incrementIndex(grammar) != 0){
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
    if (grammar.at(index) == ';'){
      lineNum++;
      pos = 0;
    }

    Gtoken token;

    token.type = Symbol;
    token.lexeme = grammar.at(index);
    token.lineNum = lineNum;
    token.pos = pos;
    token.error = NONE;

    incrementIndex(grammar);

    return token;
  }

  //Token is a non terminal or a token type
  else if(isalpha(grammar.at(index)) || grammar.at(index) == '_'){
    int startPos = pos;
    string lexeme;
    lexeme += grammar.at(index);
    if (incrementIndex(grammar) != 0) return makeErrorToken(UnexpectedEOF, "");
    
    while (isalpha(grammar.at(index)) || isdigit(grammar.at(index)) || grammar.at(index) == '_'){
      lexeme += grammar.at(index);
      if (incrementIndex(grammar) != 0) return makeErrorToken(UnexpectedEOF, "");
    }

    if (secondPass){
      Error err = checkUndefinedError(lexeme);

      if (err == NONE){
        Gtoken token;
        token.lexeme = lexeme;
        token.lineNum = lineNum;
        token.pos = startPos;
        token.error = NONE;
       
        if (listOfNonTerminals.count(lexeme)){
          token.type = Non_Terminal;
        }

        else{
          token.type = Token_Type;
        }

        return token;
      }

      else{
        return makeErrorToken(err, lexeme);
      }

    }

    else{
      Gtoken token;
      token.type = Non_Terminal;
      token.lexeme = lexeme;
      token.lineNum = lineNum;
      token.pos = startPos;
      token.error = NONE;

      return token;
    }
  }

  //Token is a terminal
  else if(grammar.at(index) == '\"'){
    int startPos = pos;
    string lexeme;
    if (incrementIndex(grammar) != 0) return makeErrorToken(UnexpectedEOF, "");

    while (grammar.at(index) != '\"'){
      lexeme += grammar.at(index);
      if (incrementIndex(grammar) != 0) return makeErrorToken(UnexpectedEOF, "");
    }

    Gtoken token;

    token.type = Terminal;
    token.lexeme = lexeme;
    token.lineNum = lineNum;
    token.pos = startPos;
    token.error = NONE;

    incrementIndex(grammar);
    return token;
  }

  else{
    Gtoken tok = makeErrorToken(UnknownCharacter, string(1, grammar.at(index)));
    incrementIndex(grammar);
    return tok;
  }
}

//For each non terminal, this function calculates and stores all possible terminals that can begin it
//It Assumes for now grammar is syntactically valid, any syntactic errors are picked up by the parser
void Tokeniser::collectFirstSetInfo(){
  Gtoken token;

  token = getNextToken();
  while (token.type != END_OF_GRAMMAR){
    FirstSetInfo info;

    info.nonTerminal= token;

    token = getNextToken(); //Should consume the = symbol

    bool collectNext; //Flag that determines if the next terminal can start the non terminal

    int currentLevel = 0;
    int firstLevel = -1;  //The level where the first terminal/nonterminal was seen

    bool firstFoundInBracket = false;
    bool checkOutsideBracket = false;

    while (!(token.type == Symbol && token.lexeme == ";")){
      if (token.type == ERROR || token.type == END_OF_GRAMMAR){
        allFirstSetInfo.push_back(info);
        return;
      }

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
        info.firstTerminals.insert(token);
        collectNext = false;
      }
      token = getNextToken();
    }
    allFirstSetInfo.push_back(info);
    token = getNextToken();
    
  }
  index = 0; //Return to start of grammar
}

FirstSetInfo Tokeniser::findFirstSetInfo(Gtoken t){
  for (auto info : allFirstSetInfo){
    if (info.nonTerminal.lexeme == t.lexeme){
      return info;
    }
  }
}

//The current vector of terminals may contain non terminals
//This function will go through this vector, and convert any non terminals to the terminals or tokens that can begin it
void Tokeniser::refineFirstSetInfo(FirstSetInfo &returnInfo, FirstSetInfo &currentInfo){
  auto termPointer = currentInfo.firstTerminals.begin(); //Iterator pointing to set elements

  while(termPointer != currentInfo.firstTerminals.end()){
    Gtoken t = *termPointer;

    if (t.type == Non_Terminal || t.type == Token_Type){

      if (t.type == Token_Type || listOfNonTerminals.count(t.lexeme) == 0){ //Assume it is a token
        t.type = Token_Type;

        if (returnInfo.nonTerminal.lexeme != currentInfo.nonTerminal.lexeme){ //Then need to add token to return non terminal info
          returnInfo.firstTokens.insert(t);
          returnInfo.firstTerminals.insert(t);
        }
        currentInfo.firstTokens.insert(t);

        if (!tokenFileExists){
          listOfTokens.insert(t.lexeme);
        }

        termPointer++;
      }

      else{ //It is a non terminal
        FirstSetInfo nextInfo = findFirstSetInfo(t);
    
        refineFirstSetInfo(currentInfo, nextInfo);
        currentInfo.firstTerminals.erase(t);
        //Returned to original non terminal info, start from beginning as items in set not to the end
        termPointer = currentInfo.firstTerminals.begin();
      }
    }
    else{ //If terminal
      if (returnInfo.nonTerminal.lexeme != currentInfo.nonTerminal.lexeme){ //Then need to add terminal to return non terminal info
        returnInfo.firstTerminals.insert(t);
      }
      termPointer++;
    }
  }
}
