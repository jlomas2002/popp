#include <iostream>
#include <fstream>
#include "tokeniser.hpp"
#include "gparser.hpp"
#include "fileWriter.hpp"
#include "gui.hpp"

using namespace std;

int main(int argc, char* argv[]) {
      if (argc == 1){ //open up gui
          //setup qt app and create the main window
          QApplication app(argc, argv);
          GUI userInterface;
          userInterface.show();

          return app.exec();
      }
      else{
        if (argc != 5){
            cout<<"Incorrect number of arguments. Either supply 0 to use the GUI, or 4 to run on the command line.\n"<<endl;
            cout<<"USAGE: ./parcelona projectName targetLanguage grammarFile tokensFile.\n"<<endl; 
            cout<<"ProjectName is the name of your project, and will be used to name all generated files."<<endl; 
            cout<<"TargetLanguage is language all output files will be written in. Options are: python, cpp, java."<<endl; 
            cout<<"GrammarFile is a path to a .txt file containing your grammar in the EBNF syntax."<< endl;
            cout<<"TokensFile is either a path to a .txt file conating any lexical tokens, or NONE if no file is required."<< endl;
            return 1;
        }
        //Simply creates the parser file in the same directory
        string projectName = argv[1];
        string chosenLang = argv[2]; //output language
        string grammarFilename = argv[3];
        string tokensFilename = argv[4];

        ifstream fileReader(grammarFilename);

        //check if grammar file is txt file
        string extension = grammarFilename.substr(grammarFilename.length() - 4);
        if (grammarFilename.length() < 5 || extension != ".txt"){ //filename too short or doesn't have .txt extension
            cout<<"The grammar file must have a .txt extension"<<endl;
            return 1;
        }

        //check file can be opened/exists
        string grammar;
        if (fileReader.fail()){
            cout<<"The provided grammar file could not be opened."<<endl;
            return 1;
        }
        else{ //Read in grammar
            string rule;
            while (getline (fileReader, rule)){
                grammar.append(rule);
            }
            fileReader.close();
        }

        vector<string>validLangs = {"cpp", "python", "java"};
        //check output lang is valid
        bool valid = false;
        for (string lang : validLangs){
            if (chosenLang == lang){
                valid = true;
            }
        }

        if (!valid){
            cout<<"The requested output language is not valid."<<endl;
            cout<<"This argument must be one of the following: cpp, python, java."<<endl;
            return 1;
        }

        string tokens = "";
        string extension = tokensFilename.substr(tokensFilename.length() - 4);
        if ((tokensFilename.length() < 5 || extension != ".txt") && tokensFilename != "None"){ //filename too short or doesn't have .txt extension
            cout<<"The token file must have a .txt extension"<<endl;
            return 1;
        }

        if (tokensFilename != "None"){
            ifstream tokenFileReader(tokensFilename);
            //check file can be opened/exists
            if (tokenFileReader.fail()){
                cout<<"The provided tokens file could not be opened."<<endl;
                return 1;
            }
            else{
                string line;
                while(getline (tokenFileReader, line)){
                    tokens.append(line);
                }
                tokenFileReader.close();
            }
        }

        Tokeniser tokeniser(grammar, tokens);
        if (tokeniser.getErrorState()){
            string errorFeedback = "";
            if (tokeniser.getGrammarFileError().type == Error_Type){
                Gtoken errorTok = tokeniser.getGrammarFileError();
                errorFeedback += "Error detected in grammar file.\n";
                errorFeedback += tokeniser.errorToString(errorTok.error);
                errorFeedback += ", found " + errorTok.lexeme + " in rule " + to_string(errorTok.lineNum) + " at position " + to_string(errorTok.pos--) + ".\n";
            }
            if(tokeniser.getTokenFileError().type == Error_Type){
                Gtoken errorTok = tokeniser.getTokenFileError();
                errorFeedback += "Error detected in tokens file.\n";
                errorFeedback += tokeniser.errorToString(errorTok.error);
                errorFeedback += ", found " + errorTok.lexeme + " in token definition " + to_string(errorTok.lineNum) + " at position " + to_string(errorTok.pos--) + ".\n";
            
            }
            errorFeedback += "See gitlab page https://gitlab.com/jlomas/parcelona on how to define grammar and tokens.";
            cout<<errorFeedback
            return 1;
        }

        FileWriter fileWriter(chosenLang);
        fileWriter.fileSetup(tokeniser.getAllFirstSetInfo(), tokeniser.getListOfTokens(), tokeniser.getTokenRegexes(), projectName);
        Gtoken tok = parseGrammar(tokeniser, fileWriter);

        string extension;
        string headerExtension;
        if (chosenLang == "python"){
            extension = ".py";
            headerExtension = "";
        }
        else if (chosenLang == "cpp"){
            extension = ".cpp";
            headerExtension = ".hpp";
        }
        else if (chosenLang == "java"){
            extension = ".java";
            headerExtension = "";
        }

        string parserFilename = projectName + "Parser" + extension;
        string lexerFilename = projectName + "Lexer" + extension;
        string tokDefFilename;
        if (chosenLang == "cpp"){
            tokDefFilename = projectName + "TokDefintion" + headerExtension;
            string parserHeaderFilename = projectName + "Parser" + headerExtension;
            string lexerHeaderFilename = projectName + "Lexer" + headerExtension;
        }
        else{
            tokDefFilename = projectName + "TokDefintion" + extension;
        }


        if (tok.type != Error_Type){
            ofstream parserFile;
            parserFile.open(parserFilename.toStdString(), ios_base::out);
            parserFile<<fileWriter.getParserText();
            parserFile.close();

            ofstream lexerFile;
            lexerFile.open(lexerFilename.toStdString(), ios_base::out);
            lexerFile<<fileWriter.getLexerText();
            lexerFile.close();

            ofstream tokDefFile;
            tokDefFile.open(tokDefFilename.toStdString(), ios_base::out);
            tokDefFile<<fileWriter.getTokDefText();
            tokDefFile.close();

            if (chosenLang == "cpp"){
                ofstream parserHeaderFile;
                parserHeaderFile.open(parserHeaderFilename.toStdString(), ios_base::out);
                parserHeaderFile<<fileWriter.getParserHeaderText();
                parserHeaderFile.close();

                ofstream lexerHeaderFile;
                lexerHeaderFile.open(lexerHeaderFilename.toStdString(), ios_base::out);
                lexerHeaderFile<<fileWriter.getLexerHeaderText();
                lexerHeaderFile.close();
            }
        }
        else{
            string errorFeedback = "";
            errorFeedback += "Error detected in grammar file.\n";
            errorFeedback += tokeniser.errorToString(tok.error);
            errorFeedback += ", found " + tok.lexeme + " in rule " + to_string(tok.lineNum) + " at position " + to_string(tok.pos--) + ".\n";

            errorFeedback += "See gitlab page https://gitlab.com/jlomas/parcelona on how to define grammar and tokens.";
            cout<<errorFeedback;
            return;
        }
    }

    return 0;
        
}
