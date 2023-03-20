#include <iostream>
#include <cctype>
#include "gparser.hpp"
using namespace std;

//make parser into class?
Tokeniser t("");
FileWriter fw("", "");
//flag required so to of the same terminal checks aren't written to the file
bool startTerminal = true;
bool veryFirstElement = true;

void parseGrammar(Tokeniser &tokeniser, FileWriter writer){
    t = tokeniser;
    fw = writer;
    fw.fileSetup(t.getNonTerminalInfo());
    grammar();
}

void grammar(){
    Token tok = t.peekNextToken();
    while ((tok.type == "nonTerminal")){
        fw.writeText(tok.lexeme, "fh");
        t.getNextToken();
        rule();
        startTerminal = true;
        tok = t.peekNextToken();
    }
}

void rule(){
    Token tok = t.getNextToken();
    if (tok.lexeme == "=" && tok.type == "symbol"){
        veryFirstElement = true;
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
        cout<<"ERROR2"<<endl;
    }
}

void expression(){
    startTerminal = true;

    Token tok;
    tok = t.peekNextToken();
    if (tok.type == "terminal"){
        if (veryFirstElement){
            fw.writeText("", "peekNext");
            veryFirstElement = false;
        }

        if (fw.getCollectStartTerminalsFlag()){
            fw.addStartTerminal(tok);
        }
        fw.writeText(tok.lexeme, "ifHeader");
        fw.writeText("", "getNext"); //consume the peeked token
        term();
        fw.writeText("", "endif");
    }
    else if (tok.type == "nonTerminal"){
        if (veryFirstElement){
            fw.writeText("", "peekNext");
            veryFirstElement = false;
        }

        startTerminal = false;
        if (fw.getCollectStartTerminalsFlag()){
            fw.addStartTerminal(tok);
        }
        fw.writeText(tok.lexeme, "ifHeader_nt");
        term();
        fw.writeText("", "endif");
    }
    else{
        veryFirstElement = false;
        term();
    }

    tok = t.peekNextToken();
    while (tok.lexeme == "|" && tok.type == "symbol"){
        t.getNextToken();
        tok = t.peekNextToken();
        if (tok.type == "terminal"){
            startTerminal = true;
            if (fw.getCollectStartTerminalsFlag()){
                fw.addStartTerminal(tok);
            }
            fw.writeText(tok.lexeme, "elseif");
            fw.writeText("", "getNext"); //consume the peeked token
            term();
            fw.writeText("", "endif");
        }
        else if (tok.type == "nonTerminal"){
            startTerminal = false;
            if (fw.getCollectStartTerminalsFlag()){
                fw.addStartTerminal(tok);
            }
            fw.writeText(tok.lexeme, "elseif_nt");
            term();
            fw.writeText("", "endif");
        }
        else{
            term();
        }
        tok = t.peekNextToken();
    }

}

void term(){
    factor();
    Token tok = t.peekNextToken();
    while ((tok.type == "symbol" && (tok.lexeme == "{" || tok.lexeme == "[" || tok.lexeme == "(")) || tok.type == "terminal" || tok.type == "nonTerminal"){
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
            if (startTerminal){
                startTerminal = false;
                //do nothing 
            }
            else{
                fw.writeText("", "getNext");
                fw.writeText(tok.lexeme, "ifHeader");
                fw.writeText("", "endif");
            }
        }
    }
    else if(tok.type == "symbol" && tok.lexeme == "{"){
        fw.writeText("", "bracketSeen");
        expression();
        tok = t.getNextToken();
        if (tok.type == "symbol" && tok.lexeme == "}"){
            fw.writeText("", "zeroormany_end");
        }
        else{
            cout<<"ERROR3"<<endl;
        } 
    } 
    else if(tok.type == "symbol" && tok.lexeme == "("){
        fw.writeText("", "peekNext");
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
        fw.writeText("", "bracketSeen");
        expression();
        tok = t.getNextToken();
        if (tok.type == "symbol" && tok.lexeme == "]"){
            fw.writeText("", "zeroorone_end");
        }
        else{
            cout<<"ERROR5"<<endl;
        } 
    }
    else{
        cout<<"ERROR6"<<endl;
    } 

}


