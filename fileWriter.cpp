#include <iostream>
#include <sstream>
#include "fileWriter.hpp"
#include <vector>
using namespace std;

//------------------------------------
//Token definitions in each language
string py_tokenDef =  R"(
class Token:
  type = None
  lexeme = None
  lineNumber = None
  pos = None
  error = None
)";

string cpp_tokenDef = R"(
#include <string>

struct Token {
  Type type;
  std::string lexeme;
  int lineNum;
  int pos;
  std::string error;
};

)";

//------------------------------------

//--------------------------------------------------------
//Init lexer function and key includes for each language
string py_initLexer = R"(
input = ""
index = 0
lineNum = 1

def initLexer(filename):
  global input

  fileReader = open('filename.txt', 'r')
  input = fileReader.read()

  fileReader.close()

)";

string cpp_initLexer =  R"(
#include "tokenDef.hpp"
#include "lexer.hpp"

using namespace std;

string input = "";
int index;
int lineNum;

void initLexer(string filename){
  ifstream fileReader(filename);

  string line;
  while(getline (fileReader, line)){
          input.append(line);
  }

  fileReader.close();

  index = 0;
  lineNum = 1;

}

)";

//--------------------------------------------------------

//--------------------------------------------------
//Headers - expected that one is only needed for cpp
string cpp_lexerHeader =  R"(
#ifndef LEXER_HPP
#define LEXER_HPP
#include <string>

void initLexer();
Token getNextToken();
Token peekNextToken();

#endif

)";

//--------------------------------------------------


//---------------------------------------------
//Peek next token functions for each language
string py_lexerPeek = R"(
def peekNextToken(): 
  tmp = index
  token = Token()
  token = getNextToken()
  index = tmp
  return token
)";

string cpp_lexerPeek = R"(
Token peekNextToken(){
  int tmp = index;
  Token token;
  token = getNextToken();
  index = tmp;
  return token;
}

)";

//---------------------------------------------

//-----------------------------------------------------------------------
//Regex search for each language
string cpp_regexSearch = R"(
  if (regex_search(input.begin() + index, input.end(), match, {0})) {
    Token tok;
    tok.lexeme = match.str();
    tok.type = {1};
    tok.lineNum = lineNum;
    return tok;
  }

)";

//-----------------------------------------------------------------------

//--------------------------------------------------------------------
//Dictionary of strings used to build parser/lexer for each language
map<OutputStringType, string> py = {
  //Parser strings
  {functionHeader, "def {0}():\n"},
  {tokDeclaration, ""},
  {getNextToken, "tok = getNextToken()\n"},
  {peekNextToken, "tok = peekNextToken()\n"},
  {scopeEnd, "\n"},
  {callNonTerminal, "{0}()\n"},
  {ifHeader_terminal, "if tok.lexeme == \"{0}\":\n"},
  {ifHeader_nonTerminal, "if tok.lexeme in {0}_startTerminals:\n"},
  {ifHeader_token, "if tok.type == Type.{0}:\n"},
  {elseIfHeader_terminal, "elif tok.lexeme == \"{0}\":\n"},
  {elseIfHeader_nonTerminal, "elif tok.lexeme in {0}_startTerminals"},
  {elseIfHeader_token, "elif tok.type == Type.{0}:\n"},
  {elseHeader, "else:\n"},
  {whileHeader_begin, "while "},
  {ifHeader_begin, "if "},
  {defineStartTerminals_begin, "{0}_startTerminals = ["},
  {defineStartTokens_begin, "{0}_startTokens = ["},
  {defineStartTerminals_list, "\"{0}\", "},
  {defineStartTokens_list, "Type.{0}, "},
  {defineStartTerminals_end, "\"{0}\"]\n"},
  {defineStartTokens_end, "Type.{0}]\n"},
  {statement_terminalCheck, "tok.lexeme == \"{0}\" or "},
  {statement_tokenCheck, "tok.type == Type.{0} or "},
  {statement_nonTerminalCheck_terminal, "tok.lexeme in {0}_startTerminals or "},
  {statement_nonTerminalCheck_token, "tok.type in {0}_starttokens or "},
  {statement_terminalCheck_last, "tok.lexeme == \"{0}\":\n"},
  {statement_tokenCheck_last, "tok.type == {0}:\n"},
  {statement_nonTerminalCheck_terminal_last, "tok.lexeme in {0}_startTerminals:\n"},
  {statement_nonTerminalCheck_token_last, "tok.type in {0}_startTokens:\n"},
  {errorComment, "#ERROR: Expected one of the following: "},

  //Lexer strings
  {lexer_initFunction, py_initLexer},
  {lexer_peekNextFunction, py_lexerPeek},
  {lexer_getNextFunction_header, "def getNextToken():\n"},
  {lexer_includeRegex, "import re\n"},

  //TokenDef strings
  {tokenDefinition, py_tokenDef},

};

 map<OutputStringType, string> cpp = {
  //Parser strings
  {functionHeader, "void {0}(){\n"},
  {tokDeclaration, "Token tok;\n"},
  {getNextToken, "tok = getNextToken();\n"},
  {peekNextToken, "tok = peekNextToken();\n"},
  {scopeEnd, "}\n"},
  {callNonTerminal, "{0}();\n"},
  {ifHeader_terminal, "if (tok.lexeme == \"{0}\"){\n"},
  {ifHeader_nonTerminal, "if (contains(tok.lexeme, {0}_startTerminals)){\n"},
  {ifHeader_token, "if (tok.type == {0}){\n"},
  {elseIfHeader_terminal, "else if (tok.lexeme == \"{0}\"){\n"},
  {elseIfHeader_nonTerminal, "else if (contains(tok.lexeme, {0}_startTerminals)){\n"},
  {elseIfHeader_token, "else if (tok.type == {0}){\n"},
  {elseHeader, "else{\n"},
  {whileHeader_begin, "while ("},
  {ifHeader_begin, "if ("},
  {defineStartTerminals_begin, "string {0}_startTerminals[] = {"},
  {defineStartTokens_begin, "Type {0}_startTokens[] = {"},
  {defineStartTerminals_list, "\"{0}\", "},
  {defineStartTokens_list, "{0}, "},
  {defineStartTerminals_end, "\"{0}\"};\n"},
  {defineStartTokens_end, "{0}};\n"},
  {statement_terminalCheck, "tok.lexeme == \"{0}\" || "},
  {statement_tokenCheck, "tok.type == {0} || "},
  {statement_nonTerminalCheck_terminal, "contains(tok.lexeme, {0}_startTerminals) || "},
  {statement_nonTerminalCheck_token, "contains(tok.type, {0}_startTokens) || "},
  {statement_terminalCheck_last, "tok.lexeme == \"{0}\"){\n"},
  {statement_tokenCheck_last, "tok.type == {0}){\n"},
  {statement_nonTerminalCheck_terminal_last, "contains(tok.lexeme, {0}_startTerminals){\n"},
  {statement_nonTerminalCheck_token_last, "contains(tok.type, {0}_startTokens){\n"},
  {errorComment, "//ERROR: Expected one of the following: "},

  //Lexer strings
  {lexer_initFunction, cpp_initLexer},
  {lexer_peekNextFunction, cpp_lexerPeek},
  {lexer_getNextFunction_header, "Token getNextToken(){\n"},
  {lexer_includeRegex, "#include <regex>\n"},
  {lexer_regexDefintion, "regex {0}_regex(\"{1}\");\n"},
  {lexer_matchDeclaration, "smatch match;\n"},
  {lexer_searchCode, cpp_regexSearch},

  {lexer_headerFile, cpp_lexerHeader},

  //TokenDef strings
  {tokenDefinition, cpp_tokenDef},
  {typeEnumDefinition_begin, "enum Type {\n"},
  {typeEnumDefinition_end, "};\n"}
};

//--------------------------------------------------------------------


//Create a filewriter object and assign the filename and required language
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

//Return the firse set information for the non terminal which matches the name argument
FirstSetInfo FileWriter::getFirstSetInfo(string name){
  for (FirstSetInfo info : allFirstSetInfo){
    if (info.nonTerminal.lexeme == name){
      return info;
    }
  }
}

//Create the token definition header file
//This header should define a token struct and any token types
void FileWriter::createTokenDef(set<string> allTokenTypes){
  ofstream defFile;

  if (*language == cpp){
    defFile.open("tokenDef.cpp", ios_base::out);

    defFile<<"#ifndef TOKENDEF_HPP\n";
    defFile<<"#define TOKENDEF_HPP\n\n";

  }
  else if (*language == py){
    defFile.open("tokenDef.py", ios_base::out);
  }

  defFile<<(*language)[tokenDefinition];

  //Add token type enum if required
  if (allTokenTypes.size()>0){
    defFile<<(*language)[typeEnumDefinition_begin];
    for (string tokenType : allTokenTypes){
      defFile<<"    "<<tokenType<<",\n";
    }
    defFile<<(*language)[typeEnumDefinition_end];
  }

  defFile<<"#endif";
}

//Create a lexer file
void FileWriter::createLexer(bool makeTemplate, vector<TokenRegex> tokenRegexes){
  ofstream lexfile;

  if (*language == cpp){
    lexfile.open("lexer.cpp", ios_base::out);
  }
    else if (*language == py){
    lexfile.open("lexer.py", ios_base::out);
  }

  if (!makeTemplate){
    lexfile<<(*language)[lexer_includeRegex];
  }

  lexfile<<(*language)[lexer_initFunction];
  lexfile<<(*language)[lexer_getNextFunction_header]<<"\n";

  if (makeTemplate){
    //Add code to return next character in input
    lexfile<<(*language)[scopeEnd]<<"\n";
    lexfile<<(*language)[lexer_peekNextFunction];
  }
  else{
    string def = (*language)[lexer_regexDefintion];
    for (auto regexDef:tokenRegexes){
      def.replace(def.find("{0}"), 3, regexDef.name);
      def.replace(def.find("{1}"), 3, regexDef.regex);
      lexfile<<"    "<<def;
    }

    lexfile<<"    "<<(*language)[lexer_matchDeclaration];

    for (auto regexDef:tokenRegexes){
      string searchCode = (*language)[lexer_searchCode];
      searchCode.replace(searchCode.find("{0}"), 3, regexDef.name.append("_regex"));
      searchCode.replace(searchCode.find("{1}"), 3, regexDef.name.append("_regex"));
      lexfile<<searchCode<<"\n";
    }
    lexfile<<(*language)[scopeEnd]<<"\n";
    lexfile<<(*language)[lexer_peekNextFunction];

  }

  lexfile.close();
}

void FileWriter::fileSetup(vector<FirstSetInfo> allFSInfo, set<string> allTokenTypes, string tokenFile, vector<TokenRegex> tokenRegexes){
  //Create token definition header file to be used by lexer and parser
  createTokenDef(allTokenTypes);

  allFirstSetInfo = allFSInfo;

  //Create lexer file if required
  if (tokenFile != ""){
    if (tokenFile == "template"){
      createLexer(true, tokenRegexes);
    }
    else{
       createLexer(false, tokenRegexes);
    }
  }

  //Add list of start terminals/start tokens for each non terminal to parser file
  ofstream pfile;
  pfile.open(fileName, ios_base::out);

  for (FirstSetInfo info : allFirstSetInfo){
    auto terminalPointer = info.firstTerminals.begin();
    auto tokenPointer = info.firstTokens.begin();

    auto lookAhead = terminalPointer; //Used to check if we are on the last terminal

    if (info.firstTerminals.size() > 0){
      lookAhead++;
      
      pfile<<indentString()<<formatString((*language)[defineStartTerminals_begin], info.nonTerminal.lexeme);
      while (lookAhead != info.firstTerminals.end()){
        pfile<<formatString((*language)[defineStartTerminals_list], (*terminalPointer).lexeme); //same for all langs (so far)
        terminalPointer++;
        lookAhead++;
      }
      pfile<<formatString((*language)[defineStartTerminals_end], (*terminalPointer).lexeme);
    }

    if (info.firstTokens.size() > 0){
      lookAhead = tokenPointer;
      lookAhead++;

      pfile<<indentString()<<formatString((*language)[defineStartTokens_begin], info.nonTerminal.lexeme);
      while (lookAhead != info.firstTokens.end()){
        pfile<<formatString((*language)[defineStartTokens_list], (*tokenPointer).lexeme); //same for all langs (so far)
        tokenPointer++;
        lookAhead++;
      }
      pfile<<formatString((*language)[defineStartTokens_end], (*tokenPointer).lexeme);
    }
    pfile<<"\n";
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

void FileWriter::addStartTerminal(Gtoken token){
  vector<Gtoken>& currentArray = subCodeInfo.startTokens.back();
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

string FileWriter::createConditionStatement(){
  string statement;
  int i = 0;
  vector<Gtoken> &currentTokens = subCodeInfo.startTokens.back();
  while (i != int(currentTokens.size()-1)){
    if (currentTokens[i].type == Terminal){
      statement.append(formatString((*language)[statement_terminalCheck], currentTokens[i].lexeme));
    }
    else if (currentTokens[i].type == Non_Terminal){
      //use all non terminal info to check if a terminal can start, and if a token can start
      FirstSetInfo info = getFirstSetInfo(currentTokens[i].lexeme);
      if (info.firstTerminals.size() > 0){
        statement.append(formatString((*language)[statement_nonTerminalCheck_terminal], currentTokens[i].lexeme));
      }

      if (info.firstTokens.size() > 0){
        statement.append(formatString((*language)[statement_nonTerminalCheck_token], currentTokens[i].lexeme));
      }
    }
    else if (currentTokens[i].type == Token_Type){
      statement.append(formatString((*language)[statement_tokenCheck], currentTokens[i].lexeme));
    }
    i++;
  }
  if (currentTokens[i].type == Terminal){
    statement.append(formatString((*language)[statement_terminalCheck_last], currentTokens[i].lexeme));
  }
  else if (currentTokens[i].type == Non_Terminal){
    FirstSetInfo info = getFirstSetInfo(currentTokens[i].lexeme);

    if (info.firstTerminals.size() > 0){
      if (info.firstTokens.size() > 0){
        statement.append(formatString((*language)[statement_tokenCheck], currentTokens[i].lexeme));
      }
      statement.append(formatString((*language)[statement_nonTerminalCheck_terminal_last], currentTokens[i].lexeme));
    }
    else if (info.firstTokens.size() > 0){
      statement.append(formatString((*language)[statement_nonTerminalCheck_token_last], currentTokens[i].lexeme));
    }
  }
  else if (currentTokens[i].type == Token_Type){
    statement.append(formatString((*language)[statement_tokenCheck_last], currentTokens[i].lexeme));
  }

  return statement;
}

string FileWriter::createElseCode(){
  string code = "";

  code.append(indentString()+(*language)[elseHeader]);
  indent += 4;
  code.append(indentString()+(*language)[errorComment]);

  int length = 0;
  for (Gtoken tok : subCodeInfo.startTokens.back()){
    if (length >= 30){
      code.append("\n");
      if ((*language) == py){
        code.append(indentString()+"# ");
      }
      if ((*language) == cpp){
        code.append(indentString()+"// ");
      }
    }

    if (tok.type == Terminal){}
      code.append("a "+tok.lexeme+" character, ");
      length += 14;
    if (tok.type == Non_Terminal){
      code.append("a "+tok.lexeme+" start terminal/token, ");
      length += 25;
    }
    if (tok.type == Token_Type){
      code.append("a "+tok.lexeme+" type, ");
      length += 9;
    }
  }
  code.append("\n");

  return code;
}

void FileWriter::appendCode(string code){
  subCodeInfo.code[subCodeInfo.code.size()-2].append(code);
  string codeBlock = subCodeInfo.code.back();
  subCodeInfo.code[subCodeInfo.code.size()-2].append(codeBlock);

  //erase the subsequent code and the start tokens.
  subCodeInfo.code.pop_back();
  subCodeInfo.startTokens.pop_back();
}

void FileWriter::addNewSubCode(){
  vector<Gtoken> newlist;
  string newcode;
  subCodeInfo.startTokens.push_back(newlist); //Need to know start tokens for error feedback
  subCodeInfo.code.push_back(newcode);
}

void FileWriter::writeText(string text, Mode mode){
  string code;

  switch(mode){
    case Func_Begin :
    {
      addNewSubCode();

      indent = 0;
      code.append(formatString((*language)[functionHeader], text));
      indent += 4;
      code.append(indentString()+(*language)[tokDeclaration]);
      break;
    }

    case Func_End :
    {
      indent = 0;
      code.append((*language)[scopeEnd]);
      code.append("\n");
      break;
    }

    case NonTerminal_Seen :
    {
      code.append(indentString()+formatString((*language)[callNonTerminal], text));
      break;
    }

    case If_Begin_Terminal :
    {
      code.append(indentString()+formatString((*language)[ifHeader_terminal], text));
      indent += 4;
      break;
    }

    case If_Begin_NonTerminal :
    {
      code.append(indentString()+formatString((*language)[ifHeader_nonTerminal], text));
      indent += 4;
      break;
    }

    case If_Begin_Token :
    {
      code.append(indentString()+formatString((*language)[ifHeader_token], text));
      indent += 4;
      break;
    }

    case ElseIf_Terminal :
    {
      code.append(indentString()+formatString((*language)[elseIfHeader_terminal], text));
      indent += 4;
      break;
    }

    case ElseIf_NonTerminal :
    {
      code.append(indentString()+formatString((*language)[elseIfHeader_nonTerminal], text));
      indent += 4;
      break;
    }

    case ElseIf_Token :
    {
      code.append(indentString()+formatString((*language)[elseIfHeader_token], text));
      indent += 4;
      break;
    }

    case Scope_End :
    {
      indent -= 4;
      code.append(indentString()+(*language)[scopeEnd]);
      break;
    }

    case Else_Expr :
    {
      code = createElseCode();
      break;
    }

    case Else_Fact_Terminal :
    {
      code.append(indentString()+(*language)[elseHeader]);
      indent += 4;

      code.append(indentString()+(*language)[errorComment]);
      code.append("a " + text + " character.");
      code.append("\n");
      break;
    }

    case Else_Fact_Token :
    {
      code.append(indentString()+(*language)[elseHeader]);
      indent += 4;

      code.append(indentString()+(*language)[errorComment]);
      code.append("a " + text + " type.");
      code.append("\n");
      break;
    }

    case Bracket_Begin :
    {
      addNewSubCode();

      indent += 4;
      break;
    }

    case CurlyBracket_End :
    {
      indent -= 4;
      code.append(indentString()+(*language)[peekNextToken]);
      code.append(indentString()+(*language)[whileHeader_begin]);

      //Create the while loop
      code.append(createConditionStatement());

      appendCode(code);
      indent += 4;

      code = "";

      code.append(indentString()+(*language)[peekNextToken]);
      indent -= 4;
      code.append(indentString()+(*language)[scopeEnd]);
      break;
    }
  
    case SquareBracket_End :
    {
      indent -= 4;
      code.append(indentString()+(*language)[peekNextToken]);
      code.append(indentString()+(*language)[ifHeader_begin]);

      //Create the if statement
      code.append(createConditionStatement());

      appendCode(code);

      code = "";

      code.append(indentString()+(*language)[scopeEnd]);
      break;

    }

    case Get_NextTok :
    {
      code.append(indentString()+(*language)[getNextToken]);
      break;
    }

    case Peek_NextTok :
    {
      code.append(indentString()+(*language)[peekNextToken]);
      break;
    }
  }
  string &codeBlock = subCodeInfo.code.back();
  codeBlock.append(code);

  if (mode == Func_End){
    ofstream pfile;

    pfile.open(fileName, ios_base::app);
    pfile<<subCodeInfo.code.back();

    subCodeInfo.code.pop_back();
    subCodeInfo.startTokens.pop_back();
  }
}


