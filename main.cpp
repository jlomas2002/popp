#include <iostream>
#include <fstream>
#include "tokeniser.hpp"
using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2){
        cout<<"USAGE: Please provide a text file specifying the grammar as a command line argument" << endl; 
        return 0;
    }
    //read in the file
    ifstream fileReader(argv[1]);

    string rule;
    string grammar;
    Tokeniser tokeniser;

    while(getline (fileReader, rule)){
        grammar.append(rule);
    }

    tokeniser.setGrammar(grammar);
    Token token;
    token = tokeniser.getNextToken();
    while (token.lexeme != "EOF"){
        std::cout<<token.lexeme<<"  "<<token.type<<"  "<<token.lineNum<<std::endl;
        token = tokeniser.getNextToken();
    }
    //tokeniser.tokenise();

    // Close the grammar file
    fileReader.close();
    return 0;
}