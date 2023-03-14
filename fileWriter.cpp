#include <iostream>
#include <sstream>
#include "fileWriter.hpp"
#include <vector>
using namespace std;

//getNextToken() should return a Token object 
//where Token is a class
map<std::string, std::string> py = {
    {"functionHeader", "def {0}():\n"},
    {"getNext", "tok = getNextToken()\n"},
    {"endFunction", "\n\n"},
    {"nonTerminal", "{0}()\n"},
    {"ifHeader", "if tok.lexeme == \"{0}\":\n"},
    {"ifHeader_nt", "if tok.lexeme in {0}_startTerminals:\n"},
    {"elseIf", "elif tok.lexeme == \"{0}\":\n"},
    {"endIf", "\n"},
    {"peekNext", "tok = peekNextToken()\n"},
    {"elseIf_nt", "elif tok.lexeme in {0}_startTerminals"},
    {"whileBegin", "while "},
    {"ifBegin", "if "},
    {"scopeEnd", "\n"},
    {"defineStartTerms", "{0}_startTerminals = ["},
    {"listEnd", "\"{0}\"]\n"},
    {"terminalCheck", "tok.lexeme == \"{0}\" or "},
    {"nonTerminalCheck", "tok.lexeme in {0}_startTerminals or "},
    {"terminalCheck_last", "tok.lexeme == \"{0}\":\n"},
    {"nonTerminalCheck_last", "tok.lexeme in {0}_startTerminals:\n"},

};

 map<std::string, std::string> cpp = {
    {"functionHeader", "void {0}(){\n"},
    {"getNext", "tok = getNextToken();\n"},
    {"tokDeclar", "Token tok;\n"},
    {"endFunction", "}\n\n"},
    {"nonTerminal", "{0}();\n"},
    {"ifHeader", "if (tok.lexeme == \"{0}\"){\n"},
    {"ifHeader_nt", "if (contains(tok.lexeme, {0}_startTerminals)){\n"},
    {"elseIf", "else if (tok.lexeme == \"{0}\"){\n"},
    {"endIf", "}\n"},
    {"peekNext", "tok = peekNextToken();\n"},
    {"elseIf_nt", "else if (contains(tok.lexeme, {0}_startTerminals)){\n"},
    {"whileBegin", "while ("},
    {"ifBegin", "if ("},
    {"scopeEnd", "}\n"},
    {"defineStartTerms", "string {0}_startTerminals[] = {"},
    {"listEnd", "\"{0}\"};\n"},
    {"terminalCheck", "tok.lexeme == \"{0}\" | "},
    {"nonTerminalCheck", "contains(tok.lexeme, {0}_startTerminals) | "},
    {"terminalCheck_last", "tok.lexeme == \"{0}\"){\n"},
    {"nonTerminalCheck_last", "contains(tok.lexeme, {0}_startTerminals){\n"},
};

FileWriter::FileWriter(string name, string lang){
  if (name == ""){
    ;
  }
  else{
    indent = 0;
    fileName = name;
    if (lang == "c++"){
      language = &cpp;
    }
    else if (lang == "python"){
      language = &py;
    }
  }
}

FileWriter::~FileWriter(){
}


bool FileWriter::getCollectStartTerminalsFlag(){
  if (subCodeInfo.startTokens.size() > 0){
    return true;
  }
  return false;
}

void FileWriter::addStartTerminal(Token token){
  vector<Token>& currentArray = subCodeInfo.startTokens.back();
  currentArray.push_back(token);
}

string FileWriter::formatString(string base, string text){
  string formatted = base;
  return formatted.replace(formatted.find("{0}"), 3, text);
}


string FileWriter::indentString(){
  string s;
  for (int i=0; i<indent; i++){
    s.append(" ");
  }
  return s;
}

string FileWriter::createStatement(){
  string statement;
  int i = 0;
  vector<Token> &currentTokens = subCodeInfo.startTokens.back();
  while (i != currentTokens.size()-1){
    if (currentTokens[i].type == "terminal"){
      statement.append(formatString((*language)["terminalCheck"], currentTokens[i].lexeme));
    }
    else if (currentTokens[i].type == "nonTerminal"){
      statement.append(formatString((*language)["nonTerminalCheck"], currentTokens[i].lexeme));
    }
    i++;
  }
  if (currentTokens[i].type == "terminal"){
    statement.append(formatString((*language)["terminalCheck_last"], currentTokens[i].lexeme));
  }
  else if (currentTokens[i].type == "nonTerminal"){
    statement.append(formatString((*language)["nonTerminalCheck_last"], currentTokens[i].lexeme));
  }

  return statement;
}

void FileWriter::appendCode(string code){
  ofstream pfile;

  if (subCodeInfo.code.size() == 1){//add to pfile
    pfile.open(fileName, ios_base::app);
    pfile<<code;
    string codeBlock = subCodeInfo.code[0];
    pfile<<codeBlock;

    pfile.close();
  }
  else{
    subCodeInfo.code[subCodeInfo.code.size()-2].append(code);
    string codeBlock = subCodeInfo.code.back();
    subCodeInfo.code[subCodeInfo.code.size()-2].append(codeBlock);
  }

  //erase the subsequent code and the start tokens.
  subCodeInfo.code.pop_back();
  subCodeInfo.startTokens.pop_back();
}

void FileWriter::fileSetup(vector<startTerminals> theCollections){
  ofstream pfile;
  pfile.open(fileName, ios_base::out);

  for (auto collection : theCollections){
    pfile<<indentString()<<formatString((*language)["defineStartTerms"], collection.nonTerminal);
    auto termPointer = collection.terminals.begin();
    auto tp = termPointer; //used to check if we are on the last terminal
    tp++;
    while (tp != collection.terminals.end()){
      pfile<<"\""<<(*termPointer).lexeme<<"\", "; //same for all langs (so far)
      termPointer++;
      tp++;
    }
    pfile<<formatString((*language)["listEnd"], (*termPointer).lexeme);
  }
  pfile<<"\n\n";

  if ((*language) == cpp){ //python can use the 'in' command
    pfile<<"bool contains(string searchWord, vector<string> list){\n";
    pfile<<"    for (string value : list){\n";
    pfile<<"        if (value == searchWord) return true;\n";
    pfile<<"    }\n";
    pfile<<"    return false;\n";
    pfile<<"}\n\n";
  }

  pfile.close();
}

void FileWriter::writeText(string text, string mode){
  ofstream pfile;
  if (subCodeInfo.startTokens.size() == 0){
    pfile.open(fileName, ios_base::app);
  }

  string code;
  
  if (mode == ""){
    if (pfile.is_open()){
        pfile<<text<<"\n";
    }
    else{
      cout << "File could not be opened"<<endl;
    }
    if (pfile.fail() || pfile.bad()){
      cout<<"File has failed"<<endl;
    }
  }

  else if (mode == "fh"){ //function header
    string formatted = formatString((*language)["functionHeader"], text);
    pfile<<formatted;
    indent += 4;
    if ((*language) == cpp){
      pfile<<indentString()<<(*language)["tokDeclar"];
    }
    pfile<<indentString()<<(*language)["getNext"];
  }

  else if (mode == "ef"){ //end of function
    indent = 0;
    pfile<<(*language)["endFunction"];
  }

  else if (mode == "nt"){ //non terminal
    code.append(indentString()+formatString((*language)["nonTerminal"], text));
  }

  else if (mode == "getNext"){
    code.append(indentString()+(*language)["getNext"]);
  }

  else if (mode == "ifHeader"){
    code.append(indentString()+formatString((*language)["ifHeader"], text));
    indent += 4;
  }
  else if (mode == "ifHeader_nt"){
    code.append(indentString()+formatString((*language)["ifHeader_nt"], text));
    indent += 4;
  }
  else if (mode == "elseif"){
    code.append(indentString()+formatString((*language)["elseIf"], text));
    indent += 4;
  }
  else if (mode == "endif"){
    indent -= 4;
    code.append(indentString()+(*language)["endIf"]);
  }
  else if (mode == "elseif_nt"){
    code.append(indentString()+(*language)["peekNext"]);
    code.append(indentString()+formatString((*language)["elseIf_nt"], text));
    indent += 4;
  }
  else if (mode == "bracketSeen"){
    vector<Token> newlist;
    string newcode;
    subCodeInfo.startTokens.push_back(newlist);
    subCodeInfo.code.push_back(newcode);
    subCodeInfo.indents.push_back(indent); 
    indent += 4;
  }
  else if (mode == "zeroormany_end"){
    int temp = indent;
    indent = subCodeInfo.indents.back();
    code.append(indentString()+(*language)["peekNext"]);
    code.append(indentString()+(*language)["whileBegin"]);

    //the while loop
    code.append(createStatement());

    appendCode(code);
    indent = temp;

    if (subCodeInfo.code.size() == 0){
      pfile.open(fileName, ios_base::app);
      pfile<<indentString()+(*language)["peekNext"];
      indent -= 4;
      pfile<<indentString()<<(*language)["scopeEnd"];
      pfile.close();
    }

    else{
      subCodeInfo.code[subCodeInfo.code.size()-1].append(indentString()+(*language)["peekNext"]);
      indent -= 4;
      subCodeInfo.code[subCodeInfo.code.size()-1].append(indentString()+(*language)["scopeEnd"]);
    }
    subCodeInfo.indents.pop_back();
    return; //dont want to run the code at the bottom of the function
  }
  else if (mode == "zeroorone_end"){
    int temp = indent;
    indent = subCodeInfo.indents.back();
    code.append(indentString()+(*language)["peekNext"]);
    code.append(indentString()+(*language)["ifBegin"]);

    //the while loop
    code.append(createStatement());

    appendCode(code);
    indent = temp;

    if (subCodeInfo.code.size() == 0){ //can now write to file
      pfile.open(fileName, ios_base::app);
      indent -= 4;
      pfile<<indentString()<<(*language)["scopeEnd"];
      pfile.close();
      
    }
    else{
      indent -= 4;
      subCodeInfo.code[subCodeInfo.code.size()-1].append(indentString()+(*language)["scopeEnd"]);
    }
    subCodeInfo.indents.pop_back();
    return; //dont want to run the code at the bottom of the function
  }


  if (pfile.is_open()){
    pfile<<code;
    pfile.close();
  }
  else{
    string &codeBlock = subCodeInfo.code.back();
    codeBlock.append(code);
  }
}

