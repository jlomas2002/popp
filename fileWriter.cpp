#include <iostream>
#include <sstream>
#include "fileWriter.hpp"
#include <vector>

using namespace std;


//------------------------------------
//Token definitions includes
string py_tokenDefIncludes =  R"(
from enum import Enum

)";

string cpp_tokenDefIncludes = R"(
#ifndef TOKENDEF_HPP
#define TOKENDEF_HPP
#include <string>

)";

string java_tokenDefIncludes = "";

//------------------------------------

//------------------------------------
//Token definitions in each language
string py_tokenDef =  R"(
class Token:
    def __init__(self, lexeme, t, error):
        self.lexeme = lexeme
        self.type = t
        self.error = error

)";

string cpp_tokenDef = R"(
struct Token {
    Type type;
    std::string lexeme;
    std::string error;
};

)";

string java_tokenDef = R"(
public class {0}TokDefinition {
    public String lexeme;
    public _Type type;
    public String error;

    public {1}TokDefinition(String lex, _Type t, String err){
        lexeme = lex;
        type = t;
        error = err;
    }
}

)";


//------------------------------------

//------------------------------------
//Lexer includes in each language
string py_lexerIncludes =  R"(
import re
from {0}TokDefinition import Token, Type

)";

string cpp_lexerIncludes = R"(
#include <string>
#include <regex>
#include <fstream>
#include <iostream>
#include "{0}TokDefinition.hpp"
#include "{1}Lexer.hpp"

using namespace std;

)";

string java_lexerIncludes =  R"(
import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

)";
//------------------------------------

//--------------------------------------------------------
//Init lexer function
string py_initLexer = R"(
input = ""

def initLexer(filename):
    global input

    fileReader = open(filename, 'r')
    if fileReader is not None:
        input = fileReader.read()

        fileReader.close()

    else:
        print("Error: Couldn't open file.")
        exit(3);

)";

string cpp_initLexer =  R"(
string input = "";

void initLexer(string filename){
    ifstream fileReader(filename);

    if (fileReader.good()){
        string line;
        while(getline (fileReader, line)){
            input.append(line);
        }
        fileReader.close();
    }
    else{
        cout<<"Error: Couldn't open file.\n";
        exit(3);
    }
}

)";

string java_initLexer =  R"(
public class {0}Lexer{
    private String input;

    public {1}Lexer(){
        input = "";
    }

    public void initLexer(String filename){
        File file = new File(filename);
        try {
            Scanner reader = new Scanner(file);
            while (reader.hasNextLine()) {
                input += reader.nextLine();
            }
        reader.close();
        }
        catch (FileNotFoundException e) {
            System.out.println("Error: Couldn't open file.");
            System.exit(3);
        }
    }


)";

//--------------------------------------------------------

//--------------------------------------------------
//Headers - expected that one is only needed for cpp
string cpp_lexerHeader =  R"(
#ifndef LEXER_HPP
#define LEXER_HPP
#include <string>
#include "{0}TokDefinition.hpp"

void initLexer(std::string filename);
Token getNextToken();
Token peekNextToken();

#endif

)";

//--------------------------------------------------

//---------------------------------------------
//Peek next token functions for each language
string py_lexerPeek = R"(
def peekNextToken(): 
    global input
    tmp = input
    token = getNextToken()
    input = tmp
    return token

)";

string cpp_lexerPeek = R"(
Token peekNextToken(){
    string tmp = input;
    Token token;
    token = getNextToken();
    input = tmp;
    return token;
}

)";

string java_lexerPeek = R"(
    public {0}TokDefinition peekNextToken(){
        String tmp = input;
        {1}TokDefinition token = getNextToken();
        input = tmp;
        return token;
    }
}

)";

//---------------------------------------------


//-----------------------------------------------------------------------
//Regex search for each language
string py_regexSearch = R"(
    match = re.search({0}, input)

    if match:
        lexeme = match.group()
        tok = Token(lexeme, Type.{1}, "")
        input = input[len(lexeme):]
        return tok

)";

string cpp_regexSearch = R"(
    if (regex_search(input, match, {0})) {
        tok.lexeme = match.str();
        tok.type = {1};

        input.erase(0, match.str().length()); //Consume the input

        return tok;
    }

)";

string java_regexSearch = R"(
        Matcher matcher = {0}.matcher(input);
        if (matcher.find()) {
            String lexeme = matcher.group();
            {0}TokDefinition tok = new {1}TokDefinition(lexeme, _Type.{1}, "");
            input = input.substring(lexeme.length());

            return tok;
        }

)";

//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
//Skip whitespace
string py_skipWhitespace = R"(
def getNextToken():
    global input

    if (len(input) == 0):
        return Token("", Type.ENDOFINPUT, "")

    while input[0].isspace():
        input = input[1:]
        if (len(input) == 0):
            return Token("", Type.ENDOFINPUT, "")

)";

string cpp_skipWhitespace = R"(
Token getNextToken(){
    Token tok;

    if (input.length() == 0){
        tok.lexeme = "";
        tok.error = "";
        tok.type = ENDOFINPUT;
        return tok;
    }

    while(isspace(input.at(0))){
        input.erase(0, 1);
        if (input.length() == 0){
            tok.lexeme = "";
            tok.error = "";
            tok.type = ENDOFINPUT;
            return tok;
        }
    }

)";

string java_skipWhitespace = R"(
    public {0}TokDefinition getNextToken(){

        if (input.length() == 0){
            {1}TokDefinition tok = new {2}TokDefinition("", _Type.ENDOFINPUT, "");
            return tok;
        }

        while(Character.isWhitespace(input.charAt(0))){
            input = input.substring(1);
            if (input.length() == 0){
                {3}TokDefinition tok = new {4}TokDefinition("", _Type.ENDOFINPUT, "");
                return tok;
            }
        }

)";
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
//Return next char
string py_returnNextChar = R"(
    tok = Token(input[0], Type.NONE, "")
    input = input[1:]
    return tok

)";

string cpp_returnNextChar = R"(
    tok.lexeme = string(1, input.at(0));
    tok.error = "";
    tok.type = NONE;

    input.erase(0, 1);
    return tok;
}

)";

string java_returnNextChar = R"(
        {0}TokDefinition tok = new {1}TokDefinition(Character.toString(input.charAt(0)), _Type.NONE, "");
        input = input.substring(1);
        return tok;
    }

)";


//-----------------------------------------------------------------------

//--------------------------------------------------
//Includes for parser file
string py_parserIncludes =  R"(
from {0}TokDefinition import Token, Type
from {1}Lexer import initLexer, getNextToken, peekNextToken
import sys

)";


string cpp_parserIncludes =  R"(
#include <iostream>
#include <string>
#include <vector>
#include "{0}Parser.hpp"
#include "{1}TokDefinition.hpp"
#include "{2}Lexer.hpp"

using namespace std;

)";

string java_parserIncludes = R"(
import java.util.Arrays;

public class {0}Parser{
    {1}Lexer lx;

)";

//--------------------------------------------------

//--------------------------------------------------
//Main function for parser file
string py_parserMain =  R"(
if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Error: Must supply one file to parse")
        exit(4)

    initLexer(sys.argv[1])
    {0}()
    print("Text is valid!")
    exit(0)

)";


string cpp_parserMain =  R"(
int main(int argc, char* argv[]){
    if (argc != 2){
        cout<<"Error: Must supply one file to parse.\n";
        exit(4);
    }

    initLexer(argv[1]);
    {0}();
    cout<<"Text is valid!\n";
    exit(0);
}

)";

string java_parserMain =  R"(
    public static void main(String[] args){
        if (args.length != 1){
            System.out.println("Error: Must supply one file to parse.");
            System.exit(4);
        }

        {0}Parser parser = new {1}Parser();
        parser.lx = new {2}Lexer();
        parser.lx.initLexer(args[0]);
        parser.{3}();
        System.out.println("Text is valid!");
        System.exit(0);
    }
}
)";

//--------------------------------------------------


//--------------------------------------------------------------------
//Dictionary of strings used to build parser/lexer for each language
map<OutputStringType, string> py = {
  //Parser strings
  {functionHeader, "def {0}():\n"},
  {tokDeclaration, "\n"},
  {getNextToken, "tok = getNextToken()\n"},
  {peekNextToken, "tok = peekNextToken()\n"},
  {scopeEnd, "\n"},
  {addActionComment, "#Add possible action\n"},
  {pythonPass, "pass\n"},
  {callNonTerminal, "{0}()\n"},
  {ifHeader_terminal, "if tok.lexeme == \"{0}\":\n"},
  {ifHeader_nonTerminal, "if tok.lexeme in {0}_firstTerminals or tok.type in {1}_firstTokens:\n"},
  {ifHeader_token, "if tok.type == Type.{0}:\n"},
  {elseIfHeader_terminal, "elif tok.lexeme == \"{0}\":\n"},
  {elseIfHeader_nonTerminal, "elif tok.lexeme in {0}_firstTerminals or tok.type in {1}_firstTokens:\n"},
  {elseIfHeader_token, "elif tok.type == Type.{0}:\n"},
  {elseHeader, "else:\n"},
  {whileHeader_begin, "while "},
  {ifHeader_begin, "if "},
  {defineFirstTerminals_begin, "{0}_firstTerminals = ["},
  {defineFirstTokens_begin, "{0}_firstTokens = ["},
  {defineFirstTerminals_list, "\"{0}\", "},
  {defineFirstTokens_list, "Type.{0}, "},
  {defineFirstTerminals_end, "\"{0}\"]\n"},
  {defineFirstTokens_end, "Type.{0}]\n"},
  {listEnd, "]\n"},
  {statement_terminalCheck, "tok.lexeme == \"{0}\" or "},
  {statement_tokenCheck, "tok.type == Type.{0} or "},
  {statement_nonTerminalCheck_terminal, "tok.lexeme in {0}_firstTerminals or "},
  {statement_nonTerminalCheck_token, "tok.type in {0}_firstTokens or "},
  {statement_terminalCheck_last, "tok.lexeme == \"{0}\":\n"},
  {statement_tokenCheck_last, "tok.type == Type.{0}:\n"},
  {statement_nonTerminalCheck_terminal_last, "tok.lexeme in {0}_firstTerminals:\n"},
  {statement_nonTerminalCheck_token_last, "tok.type in {0}_firstTokens:\n"},
  {errorString_begin, "errorString = \"ERROR: Expected one of the following: \"\n"},
  {errorString_append, "errorString += \" "},
  {errorString_end, "\"\n"},
  {errorOutput, "print(errorString)\n"},
  {exitWithError, "exit(2)\n"},
  {parserIncludes, py_parserIncludes},
  {parserMain, py_parserMain},

  //Lexer strings
  {lexer_includes, py_lexerIncludes},
  {lexer_initFunction, py_initLexer},
  {lexer_peekNextFunction, py_lexerPeek},
  {lexer_skipWhitespace, py_skipWhitespace},
  {lexer_regexDefintion, "{0}_regex = \"{1}\"\n"},
  {lexer_matchDeclaration, "\n"},
  {lexer_searchCode, py_regexSearch},
  {lexer_returnNextChar, py_returnNextChar},

  //TokenDef strings
  {tokenDefinitionIncludes, py_tokenDefIncludes},
  {tokenDefinition, py_tokenDef},
  {typeEnumDefinition_begin, "class Type(Enum):\n"},
  {typeEnumDefinition_end, ""}

};

 map<OutputStringType, string> cpp = {
  //Parser strings
  {functionHeader, "void {0}(){\n"},
  {tokDeclaration, "Token tok;\n"},
  {getNextToken, "tok = getNextToken();\n"},
  {peekNextToken, "tok = peekNextToken();\n"},
  {scopeEnd, "}\n"},
  {addActionComment, "//Add possible action\n"},
  {pythonPass, "\n"},
  {callNonTerminal, "{0}();\n"},
  {ifHeader_terminal, "if (tok.lexeme == \"{0}\"){\n"},
  {ifHeader_nonTerminal, "if (containsString(tok.lexeme, {0}_firstTerminals) || containsType(tok.type, {1}_firstTokens)){\n"},
  {ifHeader_token, "if (tok.type == {0}){\n"},
  {elseIfHeader_terminal, "else if (tok.lexeme == \"{0}\"){\n"},
  {elseIfHeader_nonTerminal, "else if (containsString(tok.lexeme, {0}_firstTerminals) || containsType(tok.type, {1}_firstTokens)){\n"},
  {elseIfHeader_token, "else if (tok.type == {0}){\n"},
  {elseHeader, "else{\n"},
  {whileHeader_begin, "while ("},
  {ifHeader_begin, "if ("},
  {defineFirstTerminals_begin, "vector<string> {0}_firstTerminals = {"},
  {defineFirstTokens_begin, "vector<Type> {0}_firstTokens = {"},
  {defineFirstTerminals_list, "\"{0}\", "},
  {defineFirstTokens_list, "{0}, "},
  {defineFirstTerminals_end, "\"{0}\"};\n"},
  {defineFirstTokens_end, "{0}};\n"},
  {listEnd, "};\n"},
  {statement_terminalCheck, "tok.lexeme == \"{0}\" || "},
  {statement_tokenCheck, "tok.type == {0} || "},
  {statement_nonTerminalCheck_terminal, "containsString(tok.lexeme, {0}_firstTerminals) || "},
  {statement_nonTerminalCheck_token, "containsType(tok.type, {0}_firstTokens) || "},
  {statement_terminalCheck_last, "tok.lexeme == \"{0}\"){\n"},
  {statement_tokenCheck_last, "tok.type == {0}){\n"},
  {statement_nonTerminalCheck_terminal_last, "containsString(tok.lexeme, {0}_firstTerminals)){\n"},
  {statement_nonTerminalCheck_token_last, "containsType(tok.type, {0}_firstTokens)){\n"},
  {errorString_begin, "string errorString = \"ERROR: Expected one of the following: \";\n"},
  {errorString_append, "errorString += \" "},
  {errorString_end, "\";\n"},
  {errorOutput, "cout<<errorString;\n"},
  {exitWithError, "exit(2);\n"},
  {parserIncludes, cpp_parserIncludes},
  {parserMain, cpp_parserMain},

  //Lexer strings
  {lexer_includes, cpp_lexerIncludes},
  {lexer_initFunction, cpp_initLexer},
  {lexer_peekNextFunction, cpp_lexerPeek},
  {lexer_skipWhitespace, cpp_skipWhitespace},
  {lexer_regexDefintion, "regex {0}_regex(\"{1}\");\n"},
  {lexer_matchDeclaration, "    smatch match;\n"},
  {lexer_searchCode, cpp_regexSearch},
  {lexer_returnNextChar, cpp_returnNextChar},

  {lexer_headerFile, cpp_lexerHeader},

  //TokenDef strings
  {tokenDefinitionIncludes, cpp_tokenDefIncludes},
  {tokenDefinition, cpp_tokenDef},
  {typeEnumDefinition_begin, "enum Type{\n"},
  {typeEnumDefinition_end, "};\n"}
};

 map<OutputStringType, string> java = {
  //Parser strings
  {functionHeader, "private void {0}(){\n"},
  {tokDeclaration, "{0}TokDefinition tok;\n"},
  {getNextToken, "tok = lx.getNextToken();\n"},
  {peekNextToken, "tok = lx.peekNextToken();\n"},
  {scopeEnd, "}\n"},
  {addActionComment, "//Add possible action\n"},
  {pythonPass, "\n"},
  {callNonTerminal, "{0}();\n"},
  {ifHeader_terminal, "if (tok.lexeme.equals(\"{0}\")){\n"},
  {ifHeader_nonTerminal, "if (Arrays.asList({0}_firstTerminals).contains(tok.lexeme) || Arrays.asList({1}_firstTokens).contains(tok.type)){\n"},
  {ifHeader_token, "if (tok.type == _Type.{0}){\n"},
  {elseIfHeader_terminal, "else if (tok.lexeme.equals(\"{0}\")){\n"},
  {elseIfHeader_nonTerminal, "else if (Arrays.asList({0}_firstTerminals).contains(tok.lexeme) || Arrays.asList({1}_firstTokens).contains(tok.type)){\n"},
  {elseIfHeader_token, "else if (tok.type == _Type.{0}){\n"},
  {elseHeader, "else{\n"},
  {whileHeader_begin, "while ("},
  {ifHeader_begin, "if ("},
  {defineFirstTerminals_begin, "    String[] {0}_firstTerminals = {"},
  {defineFirstTokens_begin, "    _Type[] {0}_firstTokens = {"},
  {defineFirstTerminals_list, "\"{0}\", "},
  {defineFirstTokens_list, "_Type.{0}, "},
  {defineFirstTerminals_end, "\"{0}\"};\n"},
  {defineFirstTokens_end, "_Type.{0}};\n"},
  {listEnd, "};\n"},
  {statement_terminalCheck, "tok.lexeme.equals(\"{0}\") || "},
  {statement_tokenCheck, "tok.type == _Type.{0} || "},
  {statement_nonTerminalCheck_terminal, "Arrays.asList({0}_firstTerminals).contains(tok.lexeme) || "},
  {statement_nonTerminalCheck_token, "Arrays.asList({0}_firstTokens).contains(tok.type) || "},
  {statement_terminalCheck_last, "tok.lexeme.equals(\"{0}\")){\n"},
  {statement_tokenCheck_last, "tok.type == _Type.{0}){\n"},
  {statement_nonTerminalCheck_terminal_last, "Arrays.asList({0}_firstTerminals).contains(tok.lexeme)){\n"},
  {statement_nonTerminalCheck_token_last, "Arrays.asList({0}_firstTokens).contains(tok.type)){\n"},
  {errorString_begin, "String errorString = \"ERROR: Expected one of the following: \";\n"},
  {errorString_append, "errorString += \" "},
  {errorString_end, "\";\n"},
  {errorOutput, "System.out.println(errorString);\n"},
  {exitWithError, "System.exit(2);\n"},
  {parserIncludes, java_parserIncludes},
  {parserMain, java_parserMain},

  //Lexer strings
  {lexer_includes, java_lexerIncludes},
  {lexer_initFunction, java_initLexer},
  {lexer_peekNextFunction, java_lexerPeek},
  {lexer_skipWhitespace, java_skipWhitespace},
  {lexer_regexDefintion, "Pattern {0}_regex = Pattern.compile(\"{0}\");\n"},
  {lexer_matchDeclaration, "\n"},
  {lexer_searchCode, java_regexSearch},
  {lexer_returnNextChar, java_returnNextChar},

  //TokenDef strings
  {tokenDefinitionIncludes, java_tokenDefIncludes},
  {tokenDefinition, java_tokenDef},
  {typeEnumDefinition_begin, "enum _Type{\n"},
  {typeEnumDefinition_end, "}\n"}
};

//--------------------------------------------------------------------


//Create a filewriter object and assign the filename and required language
FileWriter::FileWriter(string lang){
  if (lang == ""){
    ;
  }
  else{
    indent = 0;
    if (lang == "cpp"){
      language = &cpp;
    }
    else if (lang == "py"){
      language = &py;
    }
    else if (lang == "java"){
        indent += 4;
        language = &java;
    }
  }

  parserText = "";
  lexerText = "";
  tokDefText = "";
  parserHeaderText = "";
  lexerHeaderText = "";
}

FileWriter::~FileWriter(){
}

string FileWriter::getParserText(){
    return parserText;
}

string FileWriter::getLexerText(){
    return lexerText;
}

string FileWriter::getTokDefText(){
    return tokDefText;
}

string FileWriter::getParserHeaderText(){
    return parserHeaderText;
}

string FileWriter::getLexerHeaderText(){
    return lexerHeaderText;
}


//Return the firse set information for the non terminal which matches the name argument
FirstSetInfo FileWriter::getFirstSetInfo(string name){
  for (FirstSetInfo info : allFirstSetInfo){
    if (info.nonTerminal.lexeme == name){
      return info;
    }
  }
  FirstSetInfo f;
  return f;
}

//Create the token definition header file
//This header should define a token struct and any token types
void FileWriter::createTokenDef(set<string> allTokenTypes){
  tokDefText += (*language)[tokenDefinitionIncludes];

  //Add token type enum
  tokDefText += (*language)[typeEnumDefinition_begin];
  int count = 0;
  if (allTokenTypes.size()>0){
    for (string tokenType : allTokenTypes){
      tokDefText += "    " + tokenType;
      if (*language == cpp || *language == java){
          tokDefText += ",";
      }
      else if (*language == py){
          tokDefText += " = " + to_string(count);
          count ++;
      }
      tokDefText += "\n";
    }
  }
  tokDefText += "    NONE";
  if (*language == cpp || *language == java){
      tokDefText += ",";
  }
  else if (*language == py){
      tokDefText += " = " + to_string(count);
      count++;
  }
  tokDefText += "\n";
  tokDefText += "    ENDOFINPUT";
  if (*language == py){
      tokDefText += " = " + to_string(count);
  }
  tokDefText += "\n";
  tokDefText += (*language)[typeEnumDefinition_end];

  string tokDef = (*language)[tokenDefinition];
  if (*language == java){
      tokDef.replace(tokDef.find("{0}"), 3, projectName);
      tokDef.replace(tokDef.find("{1}"), 3, projectName);
  }
  tokDefText += tokDef;

  if (*language == cpp){
    tokDefText += "#endif";
  }
}

//Create a lexer file
void FileWriter::createLexer(vector<TokenRegex> tokenRegexes){

  //Create header file if needed
  if (*language == cpp){
      lexerHeaderText += formatString((*language)[lexer_headerFile], projectName);
  }

  string includes = (*language)[lexer_includes];

  if (*language == cpp || *language == py){
      includes.replace(includes.find("{0}"), 3, projectName);
      if (*language == cpp){
          includes.replace(includes.find("{1}"), 3, projectName);
      }
  }
  else if (*language == java){

  }
  lexerText += includes;

  //Define regexes
  if (*language == cpp || *language == py){
      for (auto regexDef : tokenRegexes){
        string def = (*language)[lexer_regexDefintion];
        def.replace(def.find("{0}"), 3, regexDef.name);
        if (regexDef.regex.at(0) != '^'){
            regexDef.regex.insert(regexDef.regex.begin(), '^');
        }
        def.replace(def.find("{1}"), 3, regexDef.regex);
        lexerText += def;
      }
  }

  string initLex = (*language)[lexer_initFunction];
  if (*language == java){
      initLex.replace(initLex.find("{0}"), 3, projectName);
      initLex.replace(initLex.find("{1}"), 3, projectName);
  }
  lexerText += initLex;

  //Skip whitespace
  string skipWs = (*language)[lexer_skipWhitespace];
  if (*language == java){
       for (int i=0; i<5; i++){
           skipWs.replace(skipWs.find("{"+to_string(i)+"}"), 3, projectName);
        }
  }
  lexerText += skipWs;

  if (*language == java){
      for (auto regexDef : tokenRegexes){
        string def = (*language)[lexer_regexDefintion];
        def.replace(def.find("{0}"), 3, regexDef.name);
        if (regexDef.regex.at(0) != '^'){
            regexDef.regex.insert(regexDef.regex.begin(), '^');
        }
        def.replace(def.find("{1}"), 3, regexDef.regex);
        lexerText += "    " + def;
      }
  }

  if (tokenRegexes.size() > 0){
    lexerText += (*language)[lexer_matchDeclaration];
  }

  //Add regex matching if needed
  for (auto regexDef : tokenRegexes){
    string searchCode = (*language)[lexer_searchCode];
    searchCode.replace(searchCode.find("{0}"), 3, regexDef.name + "_regex");
    searchCode.replace(searchCode.find("{1}"), 3, regexDef.name);
    lexerText += searchCode + "\n";
  }

  //Return next char
  string returnNext = (*language)[lexer_returnNextChar];
  if (*language == java){
      returnNext.replace(returnNext.find("{0}"), 3, projectName);
      returnNext.replace(returnNext.find("{1}"), 3, projectName);
  }
  lexerText += returnNext;

  //Add peek function
  string peek = (*language)[lexer_peekNextFunction];
  if (*language == java){
      peek.replace(peek.find("{0}"), 3, projectName);
      peek.replace(peek.find("{1}"), 3, projectName);
  }
  lexerText += peek;

}

void FileWriter::createMain(string startFunc){
    if (*language == java){
        string main = (*language)[parserMain];
        main.replace(main.find("{0}"), 3, projectName);
        main.replace(main.find("{1}"), 3, projectName);
        main.replace(main.find("{2}"), 3, projectName);
        main.replace(main.find("{3}"), 3, startFunc);
        parserText += main;
    }
    else{
        parserText += formatString((*language)[parserMain], startFunc);
    }
}

void FileWriter::createParserHeader(){
    parserHeaderText += "#ifndef PARSER_HPP\n";
    parserHeaderText += "#define PARSER_HPP\n\n";

    for (FirstSetInfo info : allFirstSetInfo){
        parserHeaderText += "void " + info.nonTerminal.lexeme + "();\n";
    }

    parserHeaderText += "\n";
    parserHeaderText += "#endif\n";
}

void FileWriter::fileSetup(vector<FirstSetInfo> allFSInfo, set<string> allTokenTypes, vector<TokenRegex> tokenRegexes, string projName){
  projectName = projName;
  //Create token definition header file to be used by lexer and parser
  createTokenDef(allTokenTypes);
  allFirstSetInfo = allFSInfo;
  if (*language == cpp){
     createParserHeader();
  }

  //Create lexer file
  createLexer(tokenRegexes);

  //Add necessary includes
  string includes = (*language)[parserIncludes];

  includes.replace(includes.find("{0}"), 3, projectName);
  includes.replace(includes.find("{1}"), 3, projectName);

  if (*language == cpp){
       includes.replace(includes.find("{2}"), 3, projectName);
  }

  parserText += includes;
  //Add list of first terminals/first tokens for each non terminal to parser file
  for (FirstSetInfo info : allFirstSetInfo){
    auto terminalPointer = info.firstTerminals.begin();
    auto tokenPointer = info.firstTokens.begin();

    auto lookAhead = terminalPointer; //Used to check if we are on the last terminal

    if (lookAhead == info.firstTerminals.end()){
        parserText += formatString((*language)[defineFirstTerminals_begin], info.nonTerminal.lexeme);
        parserText += (*language)[listEnd]; //Just closes the list and enters newline
    }
    else{
        lookAhead++;

        parserText += formatString((*language)[defineFirstTerminals_begin], info.nonTerminal.lexeme);
        while (lookAhead != info.firstTerminals.end()){
          parserText += formatString((*language)[defineFirstTerminals_list], (*terminalPointer).lexeme); //same for all langs (so far)
          terminalPointer++;
          lookAhead++;
        }
        parserText += formatString((*language)[defineFirstTerminals_end], (*terminalPointer).lexeme);
    }

    lookAhead = tokenPointer;
    if (lookAhead == info.firstTokens.end()){
      parserText += formatString((*language)[defineFirstTokens_begin], info.nonTerminal.lexeme);
      parserText += (*language)[listEnd]; //Just closes the list and enters newline;
    }
    else{
      lookAhead++;

      parserText += formatString((*language)[defineFirstTokens_begin], info.nonTerminal.lexeme);
      while (lookAhead != info.firstTokens.end()){
        parserText += formatString((*language)[defineFirstTokens_list], (*tokenPointer).lexeme); //same for all langs (so far)
        tokenPointer++;
        lookAhead++;
      }
      parserText += formatString((*language)[defineFirstTokens_end], (*tokenPointer).lexeme);
    }
    parserText += "\n";
  }

  if ((*language) == cpp){ //python can use the 'in' command
    parserText += "bool containsString(string searchWord, vector<string> list){\n";
    parserText += "    for (string value : list){\n";
    parserText += "        if (value == searchWord) return true;\n";
    parserText += "    }\n";
    parserText += "    return false;\n";
    parserText += "}\n\n";

    parserText += "bool containsType(Type searchType, vector<Type> list){\n";
    parserText += "    for (Type value : list){\n";
    parserText += "        if (value == searchType) return true;\n";
    parserText += "    }\n";
    parserText += "    return false;\n";
    parserText += "}\n\n";
  }

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
  code.append(indentString()+(*language)[errorString_begin]);

  int length = 0;
  code.append(indentString() + (*language)[errorString_append]);
  for (Gtoken tok : subCodeInfo.startTokens.back()){
    if (length >= 30){
      code.append((*language)[errorString_end]);
      code.append(indentString() + (*language)[errorString_append]);
      length = 0;
    }

    if (tok.type == Terminal){}
      code.append("a "+tok.lexeme+" character, ");
      length += 14;
    if (tok.type == Non_Terminal){
      code.append("a "+tok.lexeme+" first terminal/token, ");
      length += 25;
    }
    if (tok.type == Token_Type){
      code.append("a "+tok.lexeme+" type, ");
      length += 9;
    }
  }
  code.append((*language)[errorString_end]);
  code.append(indentString() + (*language)[errorOutput]);
  code.append(indentString() + (*language)[exitWithError]);

  return code;
}

void FileWriter::appendCode(string code){
  subCodeInfo.code[subCodeInfo.code.size()-2].append(code);
  string codeBlock = subCodeInfo.code.back();
  subCodeInfo.code[subCodeInfo.code.size()-2].append(codeBlock);

  //erase the subsequent code and the first tokens.
  subCodeInfo.code.pop_back();
  subCodeInfo.startTokens.pop_back();
}

void FileWriter::addNewSubCode(){
  vector<Gtoken> newlist;
  string newcode;
  subCodeInfo.startTokens.push_back(newlist); //Need to know first tokens for error feedback
  subCodeInfo.code.push_back(newcode);
}

void FileWriter::writeText(string text, Mode mode){
  string code;

  switch(mode){
    case Func_Begin :
    {
      addNewSubCode();

      indent = 0;

      if (*language == java){
          indent += 4;
      }
      code.append(indentString() + formatString((*language)[functionHeader], text));
      indent += 4;

      string declar = (*language)[tokDeclaration];
      if (*language == java){
        declar.replace(declar.find("{0}"), 3, projectName);
      }
      code.append(indentString()+declar);
      break;
    }

    case Func_End :
    {
      indent = 0;
      if (*language == java){
          indent = 4;
      }
      code.append(indentString()+(*language)[scopeEnd]);
      code.append("\n");
      indent = 0;
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
      string statement = (*language)[ifHeader_nonTerminal];
      statement.replace(statement.find("{0}"), 3, text);
      statement.replace(statement.find("{1}"), 3, text);
      code.append(indentString()+statement);
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
      string statement = (*language)[elseIfHeader_nonTerminal];
      statement.replace(statement.find("{0}"), 3, text);
      statement.replace(statement.find("{1}"), 3, text);
      code.append(indentString()+statement);
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

    case TerminalCheck_End :
    {
        code.append(indentString()+(*language)[addActionComment]);
        code.append(indentString()+(*language)[pythonPass]);

        indent -= 4;
        if (*language == cpp || *language == java){
            code.append(indentString()+(*language)[scopeEnd]);
        }
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

      code.append(indentString()+(*language)[errorString_begin]);
      code.append(indentString()+(*language)[errorString_append]);
      code.append("a " + text + " character.");
      code.append((*language)[errorString_end]);
      code.append(indentString()+(*language)[errorOutput]);
      code.append(indentString()+(*language)[exitWithError]);
      break;
    }

    case Else_Fact_Token :
    {
      code.append(indentString()+(*language)[elseHeader]);
      indent += 4;

      code.append(indentString()+(*language)[errorString_begin]);
      code.append(indentString()+(*language)[errorString_append]);
      code.append("a " + text + " type.");
      code.append((*language)[errorString_end]);
      code.append(indentString()+(*language)[errorOutput]);
      code.append(indentString()+(*language)[exitWithError]);
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
    parserText += subCodeInfo.code.back();

    subCodeInfo.code.pop_back();
    subCodeInfo.startTokens.pop_back();
  }
}


