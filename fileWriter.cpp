#include <iostream>
#include <sstream>
#include "fileWriter.hpp"
#include <vector>
using namespace std;

string py_tokenDef =  R"(
class Token:
  type = None
  lexeme = None
  lineNumber = None
  pos = None
  error = None
)";

string cpp_tokenDef = R"(
struct Token {
  Type type;
  std::string lexeme;
  int lineNum;
  int pos;
  std::string error;
};
)";

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

  fileReader.close();}

  index = 0;
  lineNum = 1;

}

)";

string cpp_lexerHeader =  R"(
#ifndef LEXER_HPP
#define LEXER_HPP
#include <string>

void initLexer();
Token getNextToken();
Token peekNextToken();

#endif

)";

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

string cpp_regexSearch = R"(
  if (regex_search(input.begin() + index, input.end(), match, {0})) {
      Token tok;
      tok.lexeme = match.str();
      tok.type = {1};
      tok.lineNum = lineNum;
      return tok;
  }

)";


//getNextToken() should return a Token object 
//where Token is a class
map<string, string> py = {
    //Parser strings
    {"functionHeader", "def {0}():\n"},
    {"getNext", "tok = getNextToken()\n"},
    {"tokDeclar", "tok = Token()\n"},
    {"endFunction", "\n\n"},
    {"nonTerminal", "{0}()\n"},
    {"ifHeader", "if tok.lexeme == \"{0}\":\n"},
    {"ifHeader_nt", "if tok.lexeme in {0}_startTerminals:\n"},
    {"ifHeader_tok", "if tok.type == Type.{0}:\n"},
    {"elseIf", "elif tok.lexeme == \"{0}\":\n"},
    {"else", "else:\n"},
    {"peekNext", "tok = peekNextToken()\n"},
    {"elseIf_nt", "elif tok.lexeme in {0}_startTerminals"},
    {"elseIf_tok", "elif tok.type == Type.{0}:\n"},
    {"whileBegin", "while "},
    {"ifBegin", "if "},
    {"scopeEnd", "\n"},
    {"defineStartTerminals", "{0}_startTerminals = ["},
    {"defineStartTokens", "{0}_startTokens = ["},
    {"terminalList", "\"{0}\", "},
    {"tokenList", "Type.{0}, "},
    {"terminalListEnd", "\"{0}\"]\n"},
    {"tokenListEnd", "Type.{0}]\n"},
    {"terminalCheck", "tok.lexeme == \"{0}\" or "},
    {"tokenCheck", "tok.type == Type.{0} or "},
    {"nonTerminalCheck_terminal", "tok.lexeme in {0}_startTerminals or "},
    {"nonTerminalCheck_token", "tok.type in {0}_starttokens or "},
    {"terminalCheck_last", "tok.lexeme == \"{0}\":\n"},
    {"nonTerminalCheck_last_terminal", "tok.lexeme in {0}_startTerminals:\n"},
    {"nonTerminalCheck_last_token", "tok.type in {0}_startTokens:\n"},
    {"tokenCheck_last", "tok.type == {0}:\n"},
    {"errorComment", "#ERROR: Expected one of the following: "},

    //Lexer strings
    {"initLexer", py_initLexer},
    {"lexerPeek", py_lexerPeek},
    {"lexerGetDef", "def getNextToken():\n"},
    {"includeRegex", "import re\n"},

    //TokenDef strings
    {"tokenDef", py_tokenDef},

};

 map<string, string> cpp = {
    //Parser strings
    {"functionHeader", "void {0}(){\n"},
    {"getNext", "tok = getNextToken();\n"},
    {"tokDeclar", "Token tok;\n"},
    {"endFunction", "}\n\n"},
    {"nonTerminal", "{0}();\n"},
    {"ifHeader", "if (tok.lexeme == \"{0}\"){\n"},
    {"ifHeader_nt", "if (contains(tok.lexeme, {0}_startTerminals)){\n"},
    {"ifHeader_tok", "if (tok.type == {0}){\n"},
    {"elseIf", "else if (tok.lexeme == \"{0}\"){\n"},
    {"else", "else{\n"},
    {"peekNext", "tok = peekNextToken();\n"},
    {"elseIf_nt", "else if (contains(tok.lexeme, {0}_startTerminals)){\n"},
    {"elseIf_tok", "else if (tok.type == {0}){\n"},
    {"whileBegin", "while ("},
    {"ifBegin", "if ("},
    {"scopeEnd", "}\n"},
    {"defineStartTerminals", "string {0}_startTerminals[] = {"},
    {"defineStartTokens", "Type {0}_startTokens[] = {"},
    {"terminalList", "\"{0}\", "},
    {"tokenList", "{0}, "},
    {"terminalListEnd", "\"{0}\"};\n"},
    {"tokenListEnd", "{0}};\n"},
    {"terminalCheck", "tok.lexeme == \"{0}\" || "},
    {"tokenCheck", "tok.type == {0} || "},
    {"nonTerminalCheck_terminal", "contains(tok.lexeme, {0}_startTerminals) || "},
    {"nonTerminalCheck_token", "contains(tok.type, {0}_startTokens) || "},
    {"terminalCheck_last", "tok.lexeme == \"{0}\"){\n"},
    {"nonTerminalCheck_last_terminal", "contains(tok.lexeme, {0}_startTerminals){\n"},
    {"nonTerminalCheck_last_token", "contains(tok.type, {0}_startTokens){\n"},
    {"tokenCheck_last", "tok.type == {0}){\n"},
    {"errorComment", "//ERROR: Expected one of the following: "},

    //Lexer strings
    {"initLexer", cpp_initLexer},
    {"lexerPeek", cpp_lexerPeek},
    {"lexerGetDef", "Token getNextToken(){\n"},
    {"includeRegex", "#include <regex>\n"},
    {"regexDef", "    regex {0}_regex(\"{1}\");\n"},
    {"matchObj", "smatch match;\n"},
    {"searchCode", cpp_regexSearch},

    {"header", cpp_lexerHeader},

    //TokenDef strings
    {"tokenDef", cpp_tokenDef},
    {"typeEnumBegin", "enum Type {\n"},
    {"typeEnumEnd", "};\n"}
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


void FileWriter::createTokenDef(set<string> allTokenTypes){
  ofstream defFile;

  if (*language == cpp){
    defFile.open("tokenDef.cpp", ios_base::out);

    defFile<<"#ifndef TOKENDEF_HPP\n";
    defFile<<"#define TOKENDEF_HPP\n\n";

  }
  else if (*language == py){
    defFile.open("output/tokenDef.py", ios_base::out);
  }

  defFile<<(*language)["tokenDef"];

  if (allTokenTypes.size()>0){
    defFile<<(*language)["typeEnumBegin"];
    for (string tokenType : allTokenTypes){
      defFile<<"    "<<tokenType<<",\n";
    }
    defFile<<(*language)["typeEnumEnd"];
  }

  defFile<<"#endif";
}
void FileWriter::createLexer(bool makeTemplate, vector<TokenRegex> tokenRegexes){
  ofstream lexfile;

  if (*language == cpp){
    lexfile.open("lexer.cpp", ios_base::out);
  }
    else if (*language == py){
    lexfile.open("lexer.py", ios_base::out);
  }

  if (!makeTemplate){
    lexfile<<(*language)["includeRegex"];
  }

  lexfile<<(*language)["initLexer"];
  lexfile<<(*language)["lexerGetDef"]<<"\n";

  if (makeTemplate){
    //Add code to return next character in input
    lexfile<<(*language)["scopeEnd"]<<"\n";
    lexfile<<(*language)["lexerPeek"];
  }
  else{
    string def = (*language)["regexDef"];
    for (auto regexDef:tokenRegexes){
      def.replace(def.find("{0}"), 3, regexDef.name);
      def.replace(def.find("{1}"), 3, regexDef.regex);
      lexfile<<def;
    }

    lexfile<<(*language)["matchObj"];

    for (auto regexDef:tokenRegexes){
      string searchCode = (*language)["searchCode"];
      searchCode.replace(searchCode.find("{0}"), 3, regexDef.name.append("_regex"));
      searchCode.replace(searchCode.find("{1}"), 3, regexDef.name.append("_regex"));
      lexfile<<searchCode<<"\n";
    }
    lexfile<<(*language)["scopeEnd"]<<"\n";
    lexfile<<(*language)["lexerPeek"];

  }

  lexfile.close();
}

void FileWriter::fileSetup(vector<NonTerminalInfo> allNtInfo, set<string> allTokenTypes, string tokenFile, vector<TokenRegex> tokenRegexes){
  //Create token definition header file to be used by lexer and parser
  createTokenDef(allTokenTypes);

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

  for (auto ntInfo : allNtInfo){
    auto terminalPointer = ntInfo.startingTerminals.begin();
    auto tokenPointer = ntInfo.startingTokens.begin();

    auto lookAhead = terminalPointer; //used to check if we are on the last terminal

    if (ntInfo.startingTerminals.size() > 0){
      lookAhead++;
      
      pfile<<indentString()<<formatString((*language)["defineStartTerminals"], ntInfo.nonTerminal.lexeme);
      while (lookAhead != ntInfo.startingTerminals.end()){
        pfile<<formatString((*language)["terminalList"], (*terminalPointer).lexeme); //same for all langs (so far)
        terminalPointer++;
        lookAhead++;
      }
      pfile<<formatString((*language)["terminalListEnd"], (*terminalPointer).lexeme);
    }

    if (ntInfo.startingTokens.size() > 0){
      lookAhead = tokenPointer;
      lookAhead++;

      pfile<<indentString()<<formatString((*language)["defineStartTokens"], ntInfo.nonTerminal.lexeme);
      while (lookAhead != ntInfo.startingTokens.end()){
        pfile<<formatString((*language)["tokenList"], (*tokenPointer).lexeme); //same for all langs (so far)
        tokenPointer++;
        lookAhead++;
      }
      pfile<<formatString((*language)["tokenListEnd"], (*tokenPointer).lexeme);
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
      statement.append(formatString((*language)["terminalCheck"], currentTokens[i].lexeme));
    }
    else if (currentTokens[i].type == Non_Terminal){
      //use all non terminal info to check if a terminal can start, and if a token can start
      statement.append(formatString((*language)["nonTerminalCheck_terminal"], currentTokens[i].lexeme));
      statement.append(formatString((*language)["nonTerminalCheck_token"], currentTokens[i].lexeme));
    }
    else if (currentTokens[i].type == Token_Type){
      statement.append(formatString((*language)["tokenCheck"], currentTokens[i].lexeme));
    }
    i++;
  }
  if (currentTokens[i].type == Terminal){
    statement.append(formatString((*language)["terminalCheck_last"], currentTokens[i].lexeme));
  }
  else if (currentTokens[i].type == Non_Terminal){
    statement.append(formatString((*language)["nonTerminalCheck_last_terminal"], currentTokens[i].lexeme));
    statement.append(formatString((*language)["nonTerminalCheck_last_token"], currentTokens[i].lexeme));
  }
  else if (currentTokens[i].type == Token_Type){
    statement.append(formatString((*language)["tokenCheck_last"], currentTokens[i].lexeme));
  }

  return statement;
}

void FileWriter::appendCode(string code){
  //cout<<subCodeInfo.code.size()<<"\n";

  subCodeInfo.code[subCodeInfo.code.size()-2].append(code);
  //cout<<"here12\n";
  string codeBlock = subCodeInfo.code.back();
  //cout<<"here13\n";
  subCodeInfo.code[subCodeInfo.code.size()-2].append(codeBlock);
  //cout<<"here14\n";

  //erase the subsequent code and the start tokens.
  subCodeInfo.code.pop_back();
  subCodeInfo.startTokens.pop_back();
}

void FileWriter::writeText(string text, Mode mode){
  string code;

  switch(mode){
    case Func_Begin :
    {
      //cout<<"here0\n";
      vector<Gtoken> newlist;
      string newcode;
      subCodeInfo.startTokens.push_back(newlist); //Need to know start tokens for error feedback
      subCodeInfo.code.push_back(newcode);
      //cout<<subCodeInfo.code.size()<<"\n";
      indent = 0;
      code.append(formatString((*language)["functionHeader"], text));
      indent += 4;
      code.append(indentString()+(*language)["tokDeclar"]);
      break;
    }

    case Func_End :
    {
      indent = 0;
      code.append((*language)["endFunction"]);
      break;
    }

    case NonTerminal_Seen :
    {
      code.append(indentString()+formatString((*language)["nonTerminal"], text));
      break;
    }

    case If_Begin_Terminal :
    {
      code.append(indentString()+formatString((*language)["ifHeader"], text));
      indent += 4;
      break;
    }

    case If_Begin_NonTerminal :
    {
      code.append(indentString()+formatString((*language)["ifHeader_nt"], text));
      indent += 4;
      break;
    }

    case If_Begin_Token :
    {
      code.append(indentString()+formatString((*language)["ifHeader_tok"], text));
      indent += 4;
      break;
    }

    case ElseIf_Terminal :
    {
      code.append(indentString()+formatString((*language)["elseIf"], text));
      indent += 4;
      break;
    }

    case ElseIf_NonTerminal :
    {
      code.append(indentString()+formatString((*language)["elseIf_nt"], text));
      indent += 4;
      break;
    }

    case ElseIf_Token :
    {
      code.append(indentString()+formatString((*language)["elseIf_tok"], text));
      indent += 4;
      break;
    }

    case Scope_End :
    {
      indent -= 4;
      code.append(indentString()+(*language)["scopeEnd"]);
      break;
    }

    case Else_Expr :
    {
      //cout<<"here7\n";
      code.append(indentString()+(*language)["else"]);
      indent += 4;
      code.append(indentString()+(*language)["errorComment"]);

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
      break;
    }

    case Else_Fact_Terminal :
    {
      //cout<<"here5\n";
      code.append(indentString()+(*language)["else"]);
      indent += 4;

      code.append(indentString()+(*language)["errorComment"]);
      code.append("a " + text + " character.");
      code.append("\n");
      break;
    }

    case Else_Fact_Token :
    {
    //cout<<"here6\n";
      code.append(indentString()+(*language)["else"]);
      indent += 4;

      code.append(indentString()+(*language)["errorComment"]);
      code.append("a " + text + " type.");
      code.append("\n");
      break;
    }

    case Bracket_Begin :
    {
      //cout<<"here15\n";
      vector<Gtoken> newlist;
      string newcode;
      subCodeInfo.startTokens.push_back(newlist);
      subCodeInfo.code.push_back(newcode);
      //cout<<subCodeInfo.code.size()<<"\n";
      indent += 4;
      break;
    }

    case CurlyBracket_End :
    {
      cout<<"here3\n";
      indent -= 4;
      code.append(indentString()+(*language)["peekNext"]);
      code.append(indentString()+(*language)["whileBegin"]);

      //Create the while loop
      code.append(createConditionStatement());

      appendCode(code);
      indent += 4;

      code = "";

      code.append(indentString()+(*language)["peekNext"]);
      indent -= 4;
      code.append(indentString()+(*language)["scopeEnd"]);
      break;
    }
  
    case SquareBracket_End :
    {
      cout<<"here4\n";
      indent -= 4;
      code.append(indentString()+(*language)["peekNext"]);
      code.append(indentString()+(*language)["ifBegin"]);

      //Create the while loop
      code.append(createConditionStatement());
      //cout<<"here9\n";

      appendCode(code);

      //cout<<"here10\n";

      code = "";

      code.append(indentString()+(*language)["scopeEnd"]);
      break;

      //cout<<"here11\n";
    }

    case Get_NextTok :
    {
      code.append(indentString()+(*language)["getNext"]);
      break;
    }

    case Peek_NextTok :
    {
      code.append(indentString()+(*language)["peekNext"]);
      break;
    }
  }

  //cout<<"here1\n";
  string &codeBlock = subCodeInfo.code.back();
  codeBlock.append(code);
                  //cout<<"here2\n";

  if (mode == Func_End){
    ofstream pfile;

    pfile.open(fileName, ios_base::app);
    pfile<<subCodeInfo.code.back();

    subCodeInfo.code.pop_back();
    subCodeInfo.startTokens.pop_back();
  }
}


