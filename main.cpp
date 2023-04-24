#include <iostream>
#include <fstream>
#include "tokeniser.hpp"
#include "gparser.hpp"
#include "fileWriter.hpp"
//#include "gui.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    //  if (argc == 1){ //open up gui
    //      //setup qt app and create the main window
    //      QApplication app(argc, argv);
    //      GUI userInterface;
    //      userInterface.show();

    //      return app.exec();
    //  }
    // else{
        if (argc != 4 && argc != 5){
            cout<<"Incorrect number of arguments. Either supply 0 to use the GUI, or 3 (or 4) to run on the command line.\n"<<endl;
            cout<<"USAGE: ./parcelona grammarFile.txt outputParserFilename parserLanguage (optional)tokenFile.\n"<<endl; 
            cout<<"grammarFile.txt is the text file containing the grammar."<<endl; 
            cout<<"outputParserFileName is the name the output parser file should be called."<<endl; 
            cout<<"parserLanguage is the language the output parsershould be written in."<<endl;
            cout<<"parserLanguage can be any of the following: c++, python.\n"<< endl;  
            cout<<"The optional tokenFile argument should be the name of a txt file containing tokens for a lexer to extract."<< endl;
            cout<<"Use 'template' for this argument to generate boilerplate for a lexer that you can then add to."<< endl;
            cout<<"Otherwise, define each token in the file as follows: tokenType = regularExpression;"<< endl;
            cout<<"Where regularExpression is a regex to be used to match and split the input into each Token."<< endl;
            return 1;
        }
        //Simply creates the parser file in the same directory
        string grammarFilename = argv[1];
        string parserFilename = argv[2];
        string oLanguage = argv[3]; //output language
        string tokenFile = "";

        if (argc == 5){
            tokenFile = argv[4];
        }

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
        else{
            string rule;

            while(getline (fileReader, rule)){
                grammar.append(rule);
            }
            fileReader.close();
        }

        //check output parser name is valid

        vector<string>validLangs = {"c++", "python"};
        //check output lang is valid
        bool valid = false;
        for (string lang : validLangs){
            if (oLanguage == lang){
                valid = true;
            }
        }

        if (!valid){
            cout<<"The requested output language is not valid."<<endl;
            cout<<"This argument must be one of the following: c++, python."<<endl;
            return 1;
        }

        //check tokenFile is valid
        string tokensInput = "";
        if (argc == 5 && tokenFile != "template"){
            string extension = tokenFile.substr(tokenFile.length() - 4);
            if (tokenFile.length() < 5 || extension != ".txt"){ //filename too short or doesn't have .txt extension
                cout<<"The token file must have a .txt extension"<<endl;
                return 1;
            }

            ifstream tokenFileReader(tokenFile);
            //check file can be opened/exists
            if (tokenFileReader.fail()){
                cout<<"The provided token file could not be opened."<<endl;
                return 1;
            }
            else{
                string line;

                while(getline (tokenFileReader, line)){
                    tokensInput.append(line);
                }

                tokenFileReader.close();
            }

        }

        if (oLanguage == "c++"){
            parserFilename.append(".cpp");
        }
        else if (oLanguage == "python"){
            parserFilename.append(".py");
        }

        Tokeniser tokeniser(grammar, tokensInput);
        FileWriter fileWriter(parserFilename, oLanguage);

        fileWriter.fileSetup(tokeniser.getAllFirstSetInfo(), tokeniser.getListOfTokens(), tokenFile, tokeniser.getTokenRegexes());
            
        parseGrammar(tokeniser, fileWriter);

        return 0;
        

   // }
}
