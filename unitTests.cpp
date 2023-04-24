#include <iostream>
#include <fstream>
#include "tokeniser.hpp"
#include "gparser.hpp"
#include "fileWriter.hpp"

using namespace std;


vector<string> validFiles = {"testgs/listofints", "testgs/sentence", "testgs/json", "testgs/jack" };

vector<string> tokeniserErrorGfiles = {"errortestgs/nontermRedef"};

vector<string> tokeniserErrorTfiles = {};

vector<string> tokeniserErrorGnTfiles = {"errortestgs/elementRedef", "errortestgs/elementRedef-toks",
                                         "errortestgs/tokRedef", "errortestgs/tokRedef-toks"};

vector<Error> tokeniserErrors = {RedefinedNonTerminal, RedefinedElement, RedefinedToken};
vector<int> tokeniserLineNums = {3, 1, 3};
vector<int> tokeniserPositions = {1, 1, 1};
vector<string> tokeniserLexemes = {"A", "A", "A"};
//Type is always error

vector<string> errorTokenFiles = {};

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
        while (pos != string::npos){
            Gtoken t;
            t.lexeme = line.substr(0, pos);
            info.firstTerminals.insert(t);
            line.erase(0, pos + 1);

            pos = line.find(",");
        }

        getline(fileFirstSets, line);

        pos = line.find(",");
        while (pos != string::npos){
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
                continue;
            }

            if (firstSet.firstTerminals != correctFirstSets[i].firstTerminals){
                cout<<"ERROR: Testing "<<filename<<" file, incorrect set of starting terminals for non terminal: "<<correctFirstSets[i].nonTerminal.lexeme<<"\n";
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
                continue;
            }

            if (firstSet.firstTokens != correctFirstSets[i].firstTokens){
                cout<<"ERROR: Testing "<<filename<<" file, incorrect set of starting tokens for non terminal: "<<correctFirstSets[i].nonTerminal.lexeme<<"\n";
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
                continue;
            }

            cout<<"PASSED: "<<filename<<" file, non terminal "<<correctFirstSets[i].nonTerminal.lexeme<<"\n";       
            i++;
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
    for (string filename : tokeniserErrorGfiles){
        ifstream file(filename+".txt");

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

        if (gramFileErr.type != ERROR){
            cout<<"ERROR: Testing error grammar "<<filename<<" type not set to error.\n";
        }

        if (gramFileErr.error != tokeniserErrors[i]){
            cout<<"ERROR: Testing error grammar "<<filename<<" incorrect error.\n";
        }

        if (gramFileErr.lexeme != tokeniserLexemes[i]){
            cout<<"ERROR: Testing error grammar "<<filename<<" incorrect lexeme.\n";
            cout<<"Expected: "<<tokeniserLexemes[i]<<"\n";
            cout<<"Actual: "<<gramFileErr.lexeme<<"\n";
        }

        if (gramFileErr.lineNum != tokeniserLineNums[i]){
            cout<<"ERROR: Testing error grammar "<<filename<<" incorrect lineNum.\n";
            cout<<"Expected: "<<tokeniserLineNums[i]<<"\n";
            cout<<"Actual: "<<gramFileErr.lineNum<<"\n";
        }

        if (gramFileErr.pos != tokeniserPositions[i]){
            cout<<"ERROR: Testing error grammar "<<filename<<" incorrect pos.\n";
            cout<<"Expected: "<<tokeniserPositions[i]<<"\n";
            cout<<"Actual: "<<gramFileErr.pos<<"\n";
        }


        i++;
  
    }

    //Test errors in token file alone
    for (string filename : tokeniserErrorTfiles){
        ifstream file(filename+".txt");

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

        if (gramFileErr.type != ERROR){
            cout<<"ERROR: Testing error grammar "<<filename<<" type not set to error.\n";
        }

        if (gramFileErr.error != tokeniserErrors[i]){
            cout<<"ERROR: Testing error grammar "<<filename<<" incorrect error.\n";
        }

        if (gramFileErr.lexeme != tokeniserLexemes[i]){
            cout<<"ERROR: Testing error grammar "<<filename<<" incorrect lexeme.\n";
            cout<<"Expected: "<<tokeniserLexemes[i]<<"\n";
            cout<<"Actual: "<<gramFileErr.lexeme<<"\n";
        }

        if (gramFileErr.lineNum != tokeniserLineNums[i]){
            cout<<"ERROR: Testing error grammar "<<filename<<" incorrect lineNum.\n";
            cout<<"Expected: "<<tokeniserLineNums[i]<<"\n";
            cout<<"Actual: "<<gramFileErr.lineNum<<"\n";
        }

        if (gramFileErr.pos != tokeniserPositions[i]){
            cout<<"ERROR: Testing error grammar "<<filename<<" incorrect pos.\n";
            cout<<"Expected: "<<tokeniserPositions[i]<<"\n";
            cout<<"Actual: "<<gramFileErr.pos<<"\n";
        }

        i++;
  
    }

    //Test errors in grammar and token files
    // int gram = 0;
    // int toks = 1;
    // while(gram < tokeniserErrorGnTfiles.size()){
    //     string gramFilename = tokeniserErrorGnTfiles[gram];
    //     string tokFilename = tokeniserErrorGnTfiles[toks];

    //     ifstream gramFile(gramFilename+".txt");
    //     ifstream tokFile(tokFilename+".txt");

    //     string line;
    //     string gramFileInput;
    //     string tokFileInput;

    //     while(getline (gramFile, line)){
    //         gramFileInput.append(line);
    //     }
    //     gramFile.close();

    //     while(getline (tokFile, line)){
    //         tokFileInput.append(line);
    //     }
    //     tokFile.close();

    //     Tokeniser tokeniser(gramFileInput, tokFileInput);

    //     Gtoken tokFileErr = tokeniser.getTokenFileError();
    //     Gtoken gramFileErr = tokeniser.getGrammarFileError();

    //     if (tokFileErr.error != NONE){
    //         cout<<"ERROR: Testing error grammar "<<filename<<" file, error indicated for token file.\n";
    //     }

    //     if (gramFileErr.type != ERROR){
    //         cout<<"ERROR: Testing error grammar "<<filename<<" type not set to error.\n";
    //     }

    //     if (gramFileErr.error != tokeniserErrors[i]){
    //         cout<<"ERROR: Testing error grammar "<<filename<<" incorrect error.\n";
    //     }

    //     if (gramFileErr.lexeme != tokeniserLexemes[i]){
    //         cout<<"ERROR: Testing error grammar "<<filename<<" incorrect lexeme.\n";
    //         cout<<"Expected: "<<tokeniserLexemes[i]<<"\n";
    //         cout<<"Actual: "<<gramFileErr.lexeme<<"\n";
    //     }

    //     if (gramFileErr.lineNum != tokeniserLineNums[i]){
    //         cout<<"ERROR: Testing error grammar "<<filename<<" incorrect lineNum.\n";
    //         cout<<"Expected: "<<tokeniserLineNums[i]<<"\n";
    //         cout<<"Actual: "<<gramFileErr.lineNum<<"\n";
    //     }

    //     if (gramFileErr.pos != tokeniserPositions[i]){
    //         cout<<"ERROR: Testing error grammar "<<filename<<" incorrect pos.\n";
    //         cout<<"Expected: "<<tokeniserPositions[i]<<"\n";
    //         cout<<"Actual: "<<gramFileErr.pos<<"\n";
    //     }

        //i++;
  
    //}


    cout<<"\n----End of testing tokeniser's error detection----\n";  
}

void parserTests(){
    // cout<<"\n----Beginning testing token files----\n\n";

    // for 

    // cout<<"\n----End of testing token files----\n";
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