#include <iostream>
#include <cctype>
#include "gparser.hpp"
using namespace std;

//make parser into class?
Tokeniser t("");
FileWriter fw("");

void parseGrammar(Tokeniser &tokeniser, FileWriter writer){
    t = tokeniser;
    fw = writer;
    grammar();
}

void grammar(){
    Token tok = t.peekNextToken();
    while ((tok.type == "nonTerminal")){
        fw.writeText(tok.lexeme, "fh");
        t.getNextToken();
        rule();
        tok = t.peekNextToken();
    }
}

void rule(){
    Token tok = t.getNextToken();
    if (tok.lexeme == "=" && tok.type == "symbol"){
        expression();
    }
    else{
        cout<<"ERROR1"<<endl;
    }
    tok = t.getNextToken();
    if(tok.lexeme == ";" && tok.type == "symbol"){
        fw.writeText("", "ef");
        return;
    }
    else{
        cout<<tok.lexeme<<endl;
        cout<<"ERROR2"<<endl;
    }
}

void expression(){
    term();
    Token tok = t.peekNextToken();
    while ((tok.lexeme == "|" || tok.lexeme == ",") && tok.type == "symbol"){
        t.getNextToken();
        term();
        tok = t.peekNextToken();
    }

}

void term(){
    factor();
    Token tok = t.peekNextToken();
    while ((tok.type == "symbol" && (tok.type == "{" || tok.type == "[" || tok.type == "(")) || tok.type == "terminal" || tok.type == "nonTerminal"){
        t.getNextToken();
        factor();
        tok = t.peekNextToken();
    }
}

void factor(){
    Token tok = t.getNextToken();
    if (tok.type == "terminal" || tok.type == "nonTerminal"){
        if (tok.type == "nonTerminal"){
            fw.writeText(tok.lexeme, "nt");
        }
        else{
            //add code for terminal
        }
    }
    else if(tok.type == "symbol" && tok.lexeme == "{"){
        expression();
        tok = t.getNextToken();
        if (tok.type == "symbol" && tok.lexeme == "}"){
            ;//good
        }
        else{
            cout<<"ERROR3"<<endl;
        } 
    } 
    else if(tok.type == "symbol" && tok.lexeme == "("){
        expression();
        tok = t.getNextToken();
        if (tok.type == "symbol" && tok.lexeme == ")"){
            ;//good
        }
        else{
            cout<<"ERROR4"<<endl;
        } 
    } 
    else if(tok.type == "symbol" && tok.lexeme == "["){
        expression();
        tok = t.getNextToken();
        if (tok.type == "symbol" && tok.lexeme == "]"){
            ;//good
        }
        else{
            cout<<"ERROR5"<<endl;
        } 
    }
    else{
        cout<<"ERROR6"<<endl;
    } 

}


