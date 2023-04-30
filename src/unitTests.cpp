#include <iostream>
#include <fstream>
#include "tokeniser.hpp"
#include "gparser.hpp"
#include "fileWriter.hpp"

using namespace std;

vector<string> validFiles = {"testgs/listofints", "testgs/sentence", "testgs/json", "testgs/jack" };

//Grammar files with errors detected by tokeniser
vector<string> tokeniser_errorGramFiles = {"tokeniser/redefinedNonterm"};

//Token files with errors detected by tokeniser
vector<string> tokeniser_errorTokFiles = {"tokeniser/unexpectedEOF-toks1", "tokeniser/unexpectedEOF-toks2",
                                          "tokeniser/unexpectedEOF-toks3", "tokeniser/unexpectedEOF-toks4",
                                          "tokeniser/invalidRegexId", "tokeniser/redefinedToken", 
                                          "tokeniser/expectedEquals"};

//Following represents the errors in each file
vector<Error> tokeniserErrors = {RedefinedNonTerminal, UnexpectedEOF, UnexpectedEOF, UnexpectedEOF, UnexpectedEOF, InvalidRegexId, RedefinedToken, ExpectedEquals};
vector<int> tokeniserLineNums = {3, 2, 2, 2, 3, 2, 4, 1};
vector<int> tokeniserPositions = {1, 2, 5, 8, 15, 1, 14, 7};
vector<string> tokeniserLexemes = {"ANonTerm", "", "", "", "", "5", "Token", "@"};
//Type is always error

//Grammar files with errors detected by parser
vector<string> parser_errorFiles = {"parser/expectedNonTerm", "parser/expectedEquals", "parser/expectedSemiColon",
                                    "parser/expectedCurlyBracket", "parser/expectedNormalBracket", "parser/expectedSquareBracket",
                                    "parser/unknownChar"};

//Following represents the errors in each file
vector<Error> parserErrors = {ExpectedNonTerminal, ExpectedEquals, ExpectedSemiColon, ExpectedCurlyBracket, ExpectedNormalBracket, ExpectedSquareBracket, UnknownCharacter};
vector<int> parserLineNums = {2, 2, 2, 1, 1, 2, 2};
vector<int> parserPositions = {1, 16, 20, 13, 15, 1, 16};
vector<string> parserLexemes = {"Bob", "here", "", "]", "]", ";", "@"};


//Reads in data representing the valid first sets for each file
void generateTestData(string filename, string &fileInput, vector<FirstSetInfo> &allFirstSetInfo){
    ifstream file(filename+".txt");
    ifstream fileFirstSets(filename+"-firstSets.txt");

    string line;

    while(getline (file, line)){
        fileInput.append(line);
    }
    file.close();

    //Retrieve first sets
    while(getline(fileFirstSets, line)){
        FirstSetInfo info;

        Gtoken nt;
        nt.lexeme = line;

        info.nonTerminal = nt;

        getline(fileFirstSets, line);

        int pos = line.find(",");
        while (pos != int(string::npos)){
            Gtoken t;
            t.lexeme = line.substr(0, pos);
            info.firstTerminals.insert(t);
            line.erase(0, pos + 1);

            pos = line.find(",");
        }

        getline(fileFirstSets, line);

        pos = line.find(",");
        while (pos != int(string::npos)){
            Gtoken t;
            t.lexeme = line.substr(0, pos);
            info.firstTokens.insert(t);
            line.erase(0, pos + 1);

            pos = line.find(",");
        }

        allFirstSetInfo.push_back(info);

    }
    fileFirstSets.close();

}


void firstSetTests(){
    cout<<"\n----Beginning testing first sets----\n\n";

    for (string filename : validFiles){
        string fileInput;
        vector<FirstSetInfo> correctFirstSets;

        generateTestData(filename, fileInput, correctFirstSets);
        Tokeniser tokeniser(fileInput, "");

        vector<FirstSetInfo> allFirstSets = tokeniser.getAllFirstSetInfo();

        if (allFirstSets.size() != correctFirstSets.size()){
            cout<<"ERROR: Incorrect number of first sets generated. Exiting...";
            return;
        }

        int i = 0;
        for (FirstSetInfo firstSet : allFirstSets){
            if (firstSet.nonTerminal.lexeme != correctFirstSets[i].nonTerminal.lexeme){
                cout<<"ERROR: Testing "<<filename<<" file, incorrect non terminal\n";
                cout<<"Expected: "<<correctFirstSets[i].nonTerminal.lexeme<<"\n";
                cout<<"Actual: "<<firstSet.nonTerminal.lexeme<<"\n";
                i++;
            }

            else if (firstSet.firstTerminals != correctFirstSets[i].firstTerminals){
                cout<<"ERROR: Testing "<<filename<<" file, incorrect set of first terminals for non terminal: "<<correctFirstSets[i].nonTerminal.lexeme<<"\n";
                cout<<"Expected: ";
                for (auto t : correctFirstSets[i].firstTerminals){
                    cout<<t.lexeme<<", ";
                }
                cout<<"\nActual: ";
                for (auto t :firstSet.firstTerminals){
                    cout<<t.lexeme<<", ";
                }
                cout<<"\n";
                i++;
            }

            else if (firstSet.firstTokens != correctFirstSets[i].firstTokens){
                cout<<"ERROR: Testing "<<filename<<" file, incorrect set of first tokens for non terminal: "<<correctFirstSets[i].nonTerminal.lexeme<<"\n";
                cout<<"Expected: ";
                for (auto t : correctFirstSets[i].firstTokens){
                    cout<<t.lexeme<<", ";
                }
                cout<<"\nActual: ";
                for (auto t : firstSet.firstTokens){
                    cout<<t.lexeme<<", ";
                }
                cout<<"\n";
                i++;
            }
            
            else{
                cout<<"PASSED: "<<filename<<" file, non terminal "<<correctFirstSets[i].nonTerminal.lexeme<<"\n";       
                i++;
            }
        }
        cout<<"\n";
    } 
    cout<<"\n----End of testing first sets----\n";  
}

void tokeniserTests(){
    cout<<"\n----Beginning testing the tokeniser's error detetection----\n\n";

    int i = 0;

    //Test valid files
    for (string filename : validFiles){
        ifstream file(filename+".txt");

        string line;
        string fileInput;

        while(getline (file, line)){
            fileInput.append(line);
        }
        file.close();

        Tokeniser tokeniser(fileInput, "");

        if (tokeniser.getErrorState()){
            cout<<"ERROR: Testing valid "<<filename<<" file, error incorrectly detected.\n";
        }
        else{
            cout<<"PASSED: "<<filename<<" file.\n";
        }
    }

    //Test errors in grammar file alone
    for (string filename : tokeniser_errorGramFiles){
        ifstream file("errortestgs/"+filename+".txt");

        string line;
        string fileInput;

        while(getline (file, line)){
            fileInput.append(line);
        }
        file.close();
        
        Tokeniser tokeniser(fileInput, "");

        Gtoken tokFileErr = tokeniser.getTokenFileError();
        Gtoken gramFileErr = tokeniser.getGrammarFileError();

        if (tokFileErr.error != NONE){
            cout<<"ERROR: Testing error grammar "<<filename<<" file, error indicated for token file.\n";
        }

        else if (gramFileErr.type != Error_Type){
            cout<<"ERROR: Testing error grammar "<<filename<<" type not set to error.\n";
        }

        else if (gramFileErr.error != tokeniserErrors[i]){
            cout<<"ERROR: Testing error grammar "<<filename<<" incorrect error.\n";
        }

        else if (gramFileErr.lexeme != tokeniserLexemes[i]){
            cout<<"ERROR: Testing error grammar "<<filename<<" incorrect lexeme.\n";
            cout<<"Expected: "<<tokeniserLexemes[i]<<"\n";
            cout<<"Actual: "<<gramFileErr.lexeme<<"\n";
        }

        else if (gramFileErr.lineNum != tokeniserLineNums[i]){
            cout<<"ERROR: Testing error grammar "<<filename<<" incorrect lineNum.\n";
            cout<<"Expected: "<<tokeniserLineNums[i]<<"\n";
            cout<<"Actual: "<<gramFileErr.lineNum<<"\n";
        }

        else if (gramFileErr.pos != tokeniserPositions[i]){
            cout<<"ERROR: Testing error grammar "<<filename<<" incorrect pos.\n";
            cout<<"Expected: "<<tokeniserPositions[i]<<"\n";
            cout<<"Actual: "<<gramFileErr.pos<<"\n";
        }

        else{
            cout<<"PASSED: "<<filename<<" file.\n";
        }

        i++;
  
    }

    //Test errors in token file alone
    for (string filename : tokeniser_errorTokFiles){
        ifstream tokFile("errortestgs/"+filename+".txt");
        ifstream gramFile("testgs/listofints.txt"); //Use a valid grammar, as we are testing tokens

        string line;
        string fileInput;
        string gramInput;

        while(getline (tokFile, line)){
            fileInput.append(line);
        }
        tokFile.close();

        while(getline (gramFile, line)){
            gramInput.append(line);
        }
        gramFile.close();

        Tokeniser tokeniser(gramInput, fileInput);

        Gtoken tokFileErr = tokeniser.getTokenFileError();
        Gtoken gramFileErr = tokeniser.getGrammarFileError();

        if (gramFileErr.error != NONE){
            cout<<"ERROR: Testing error tokens "<<filename<<" file, error indicated for grammar file.\n";
        }

        else if (tokFileErr.type != Error_Type){
            cout<<"ERROR: Testing error tokens "<<filename<<" type not set to error.\n";
        }

        else if (tokFileErr.error != tokeniserErrors[i]){
            cout<<"ERROR: Testing error tokens "<<filename<<" incorrect error.\n";
        }

        else if (tokFileErr.lexeme != tokeniserLexemes[i]){
            cout<<"ERROR: Testing error tokens "<<filename<<" incorrect lexeme.\n";
            cout<<"Expected: "<<tokeniserLexemes[i]<<"\n";
            cout<<"Actual: "<<tokFileErr.lexeme<<"\n";
        }

        else if (tokFileErr.lineNum != tokeniserLineNums[i]){
            cout<<"ERROR: Testing error tokens "<<filename<<" incorrect lineNum.\n";
            cout<<"Expected: "<<tokeniserLineNums[i]<<"\n";
            cout<<"Actual: "<<tokFileErr.lineNum<<"\n";
        }

        else if (tokFileErr.pos != tokeniserPositions[i]){
            cout<<"ERROR: Testing error tokens "<<filename<<" incorrect pos.\n";
            cout<<"Expected: "<<tokeniserPositions[i]<<"\n";
            cout<<"Actual: "<<tokFileErr.pos<<"\n";
        }

        else{
            cout<<"PASSED: "<<filename<<" file.\n";
        }

        i++;
  
    }

    //Test redefintition across two files
    ifstream tokFile("errortestgs/tokeniser/redefinedElement-toks.txt");
    ifstream gramFile("errortestgs/tokeniser/redefinedElement.txt"); 

    string line;
    string tokInput;
    string gramInput;

    while(getline (tokFile, line)){
        tokInput.append(line);
    }
    tokFile.close();

    while(getline (gramFile, line)){
        gramInput.append(line);
    }
    gramFile.close();

    Tokeniser tokeniser(gramInput, tokInput);

    Gtoken tokFileErr = tokeniser.getTokenFileError();
    Gtoken gramFileErr = tokeniser.getGrammarFileError();

    if (tokFileErr.error != NONE){
        cout<<"ERROR: Testing redef across files, token file should have no error as its processed first.\n";
    }

    else if (gramFileErr.type != Error_Type){
        cout<<"ERROR: Testing redef across files, grammar file doesn't have an error.\n";
    }

    else if (gramFileErr.error != RedefinedElement){
        cout<<"ERROR: Testing redef across files, grammar file has wrong error.\n";
    }

    
    else if (gramFileErr.lexeme != "ANonTerm"){
        cout<<"ERROR: Testing redef across files, grammar file has wrong lexeme.\n";
    }
    
    else if (gramFileErr.lineNum != 1){
        cout<<"ERROR: Testing redef across files, grammar file has wrong line number.\n";
    }

    else if (gramFileErr.pos != 4){
        cout<<"ERROR: Testing redef across files, grammar file has wrong position.\n";
    }

    else{
        cout<<"PASSED: Redefinition across files.\n";
    }


    cout<<"\n----End of testing tokeniser's error detection----\n";  
}

void parserTests(){
    cout<<"\n----Beginning testing parser error detection----\n\n";

    //Test parsing of valid files
    for (string filename : validFiles){
        ifstream file(filename+".txt");

        string line;
        string fileInput;

        while(getline (file, line)){
            fileInput.append(line);
        }
        file.close();

        Tokeniser tokeniser(fileInput, "");
        FileWriter fw("cpp");
        fw.fileSetup(tokeniser.getAllFirstSetInfo(), tokeniser.getListOfTokens(), tokeniser.getTokenRegexes(), "DELETEME");

        Gtoken tok = parseGrammar(tokeniser, fw);

        if (tok.type == Error_Type){
            cout<<"ERROR: Testing valid "<<filename<<" file, error incorrectly detected in parser.\n";
        }
        else if (tokeniser.getErrorState()){
            cout<<"ERROR: Testing valid "<<filename<<" file, error incorrectly detected in tokeniser.\n";
        }
        else{
            cout<<"PASSED: "<<filename<<" file.\n";
        }
    }

    int i = 0;

    //Test erroneous files
    for (string filename : parser_errorFiles){
       ifstream file("errortestgs/"+filename+".txt"); 

       string line;
       string fileInput;

        while(getline (file, line)){
            fileInput.append(line);
        }
        file.close();
        Tokeniser tokeniser(fileInput, "");
        FileWriter fw("cpp");

        fw.fileSetup(tokeniser.getAllFirstSetInfo(), tokeniser.getListOfTokens(), tokeniser.getTokenRegexes(), "DELETEME");


        Gtoken tok = parseGrammar(tokeniser, fw);

        if (tok.type != Error_Type){
            cout<<"ERROR: Testing error grammar "<<filename<<" file, type not set to error.\n";
        }
        else if (tokeniser.getErrorState()){
            cout<<"ERROR: Testing error grammar "<<filename<<" file, error incorrectly detected in tokeniser.\n";
        }
        else if (tok.error != parserErrors[i]){
            cout<<"ERROR: Testing error grammar "<<filename<<" file, incorrect error.\n";
        }
        else if (tok.lexeme != parserLexemes[i]){
            cout<<"ERROR: Testing error grammar "<<filename<<" file, incorrect lexeme.\n";
            cout<<"Expected: "<<parserLexemes[i]<<"\n";
            cout<<"Actual: "<<tok.lexeme<<"\n";
        }
        else if (tok.lineNum != parserLineNums[i]){
            cout<<"ERROR: Testing error grammar "<<filename<<" file, incorrect line number.\n";
            cout<<"Expected: "<<parserLineNums[i]<<"\n";
            cout<<"Actual: "<<tok.lineNum<<"\n";
        }
        else if (tok.pos != parserPositions[i]){
            cout<<"ERROR: Testing error grammar "<<filename<<" file, incorrect position.\n";
            cout<<"Expected: "<<parserPositions[i]<<"\n";
            cout<<"Actual: "<<tok.pos<<"\n";
        }
        else{
            cout<<"PASSED: "<<filename<<" file.\n";
        }

        i++;
    }

    //Special case unknown token - only an issue if a token file was provided
    ifstream tokFile("errortestgs/tokeniser/redefinedElement-toks.txt");
    ifstream gramFile("errortestgs/parser/undefinedTok.txt"); 

    string line;
    string tokInput;
    string gramInput;

    while(getline (tokFile, line)){
        tokInput.append(line);
    }
    tokFile.close();

    while(getline (gramFile, line)){
        gramInput.append(line);
    }
    gramFile.close();

    Tokeniser tokeniser(gramInput, tokInput);

    Gtoken tokFileErr = tokeniser.getTokenFileError();
    Gtoken gramFileErr = tokeniser.getGrammarFileError();

    FileWriter fw("cpp");
    fw.fileSetup(tokeniser.getAllFirstSetInfo(), tokeniser.getListOfTokens(), tokeniser.getTokenRegexes(), "DELETEME");

    Gtoken tok = parseGrammar(tokeniser, fw);

    if (tok.type != Error_Type){
        cout<<"ERROR: Testing unknown token, type not set to error.\n";
    }
    else if (tokeniser.getErrorState()){
        cout<<"ERROR: Testing unknown token, error incorrectly detected in tokeniser.\n";
    }
    else if (tok.error != UndefinedToken){
        cout<<"ERROR: Testing unknown token, incorrect error.\n";
    }
    else if (tok.lexeme != "C"){
        cout<<"ERROR: Testing unknown token, incorrect lexeme.\n";
        cout<<"Expected: C\n";
        cout<<"Actual: "<<tok.lexeme<<"\n";
    }
    else if (tok.lineNum != 2){
        cout<<"ERROR: Testing unknown token, incorrect line number.\n";
        cout<<"Expected: 2\n";
        cout<<"Actual: "<<tok.lineNum<<"\n";
    }
    else if (tok.pos != 6){
        cout<<"ERROR: Testing unknown token, incorrect position.\n";
        cout<<"Expected: 6\n";
        cout<<"Actual: "<<tok.pos<<"\n";
    }
    else{
        cout<<"PASSED: Testing unknown token.\n";
    }


    cout<<"\n----End of testing parser error detection----\n";
}

int main(int argc, char* argv[]) {
    int choice;

    cout << "Please choose an option:\n\n";
    cout << "   1. Run all unit tests.\n";
    cout << "   2. Run first set unit tests.\n";
    cout << "   3. Run tokeniser unit tests\n";
    cout << "   4. Run parser unit tests\n";
    cout<< "\n>>>";
    cin >> choice;

    if (choice < 1 || choice > 4){
      cout << "Invalid input. Closing..." << endl;
      return 0;
    }

    switch(choice){
        case 1:
        {
            firstSetTests();
            tokeniserTests();
            parserTests();
            break;
        }

        case 2:
        {
            firstSetTests();
            break;
        }

        case 3:
        {
            tokeniserTests();
            break;
        }

        case 4:
        {
            parserTests();
            break;
        }
    }

    cout<<"\n*Exiting tester*\n";

    return 0;
}
