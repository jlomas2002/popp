#include <iostream>
#include "fileWriter.hpp"
#include <vector>
using namespace std;

FileWriter::FileWriter(string name){
  if (name == ""){
    ;
  }
  else{
    indent = 0;
    fileName = name;
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
      statement.append("tok.lexeme == \""+currentTokens[i].lexeme+"\" | ");
    }
    else if (currentTokens[i].type == "nonTerminal"){
      statement.append("contains(tok.lexeme, "+currentTokens[i].lexeme+"_startTerminals){\n");
    }
    i++;
  }
  if (currentTokens[i].type == "terminal"){
    statement.append("tok.lexeme == \""+currentTokens[i].lexeme+"\"){\n");
  }
  else if (currentTokens[i].type == "nonTerminal"){
    statement.append("contains(tok.lexeme, "+currentTokens[i].lexeme+"_startTerminals){\n");
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
    pfile<<indentString()<<"string "<<collection.nonTerminal<<"_startTerminals[] = {";
    auto termPointer = collection.terminals.begin();
    auto tp = termPointer; //used to check if we are on the last terminal
    tp++;
    while (tp != collection.terminals.end()){
      pfile<<"\""<<(*termPointer).lexeme<<"\", ";
      termPointer++;
      tp++;
    }
    pfile<<"\""<<(*termPointer).lexeme<<"\"};\n";
  }
  pfile<<"\n\n";

  pfile<<"bool contains(string searchWord, vector<string> list){\n";
  pfile<<"    for (string value : list){\n";
  pfile<<"        if (value == searchWord) return true;\n";
  pfile<<"    return false;\n\n";

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
    pfile<<"void "<<text<<"(){\n";
    indent += 4;
    pfile<<indentString()<<"Token tok = getNextToken();\n";
  }

  else if (mode == "ef"){ //end of function
    indent = 0;
    pfile<<"}\n\n";
  }

  else if (mode == "nt"){ //non terminal
    code.append(indentString()+text+"();\n");
  }
  else if (mode == "getNext"){
    code.append(indentString()+"tok = getNextToken;\n");
  }
  else if (mode == "ifHeader"){
    code.append(indentString()+"if (tok.lexeme == \""+text+"\"){\n");
    indent += 4;
  }
    else if (mode == "ifHeader_nt"){
    code.append(indentString()+"if (contains(tok.lexeme, "+text+"_startTerminals)){\n");
    indent += 4;
  }
  else if (mode == "elseif"){
    code.append(indentString()+"else if (tok.lexeme == \""+text+"\"){\n");
    indent += 4;
  }
  else if (mode == "endif"){
    //cout<<"current indent 15: "<<to_string(indent)+"\n";
    indent -= 4;
    //cout<<"current indent 16: "<<to_string(indent)+"\n";
    code.append(indentString()+"}\n");
  }
  else if (mode == "elseif_nt"){
    code.append(indentString()+"tok = peekNextToken();\n");
    code.append(indentString()+"else if (contains(tok.lexeme, "+text+"_startTerminals)){\n");
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
    code.append(indentString()+"tok = peekNextToken();\n");
    code.append(indentString()+"while (");

    //the while loop
    code.append(createStatement());


    appendCode(code);
    indent = temp;

    if (subCodeInfo.code.size() == 0){
      pfile.open(fileName, ios_base::app);
      //cout<<"current indent 1: "<<to_string(indent)+"\n";
      pfile<<indentString()+"tok = peekNextToken();\n";
      indent -= 4;
      //cout<<"current indent 2: "<<to_string(indent)+"\n";
      pfile<<indentString()<<"}\n";
      //indent -= 4;
      //cout<<"current indent 3: "<<to_string(indent)+"\n";
      pfile.close();
    }

    else{
      //cout<<"current indent 4: "<<to_string(indent)+"\n";
      subCodeInfo.code[subCodeInfo.code.size()-1].append(indentString()+"tok = peekNextToken();\n");
      indent -= 4;
      //cout<<"current indent 5: "<<to_string(indent)+"\n";
      subCodeInfo.code[subCodeInfo.code.size()-1].append(indentString()+"}\n");
      //indent -= 4;
      //cout<<"CURRENT INDENT AFTER WHILE LOOP END = " + to_string(indent) +"\n";
    }
    subCodeInfo.indents.pop_back();
    return; //dont want to run the code at the bottom of the function
  }
  else if (mode == "zeroorone_end"){
    int temp = indent;
    indent = subCodeInfo.indents.back();
    code.append(indentString()+"tok = peekNextToken();\n");
    code.append(indentString()+"if (");

    //the while loop
    code.append(createStatement());

    appendCode(code);
    indent = temp;

    if (subCodeInfo.code.size() == 0){ //can now write to file
      pfile.open(fileName, ios_base::app);
      //cout<<"current indent 7: "<<to_string(indent)+"\n";
      indent -= 4;
      //cout<<"current indent 8: "<<to_string(indent)+"\n";
      pfile<<indentString()<<"}\n";
      //indent -= 4;
      //cout<<"CURRENT INDENT AFTER [] END = " + to_string(indent) +"\n";
      pfile.close();
      
    }
    else{
      //cout<<"current indent 9: "<<to_string(indent)+"\n";
      indent -= 4;
      //cout<<"current indent 10: "<<to_string(indent)+"\n";
      subCodeInfo.code[subCodeInfo.code.size()-1].append(indentString()+"}\n");
      //indent -= 4;
      //cout<<"current indent 11: "<<to_string(indent)+"\n";
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

