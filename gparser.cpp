#include <iostream>
#include <cctype>
#include "gparser.hpp"
using namespace std;

//make parser into class?
Tokeniser t("");
FileWriter fw("");
//flag required so to of the same terminal checks aren't written to the file
bool startTerminal = true;

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
        //cout<<"ENTERING EXPRESSION()\n";
    Token tok;
    tok = t.peekNextToken();
    if (tok.type == "terminal"){
        if (fw.getCollectStartTerminalsFlag()){
            fw.addStartTerminal(tok);
        }
        cout<<"in expression(), peeked tok is terminal, "+tok.lexeme+" creating if header\n";
        fw.writeText(tok.lexeme, "ifHeader");
        term();
        //cout<<"returned from term(), writing endif\n";
        fw.writeText("", "endif");
    }
    else if (tok.type == "nonTerminal"){
        startTerminal = false;
        if (fw.getCollectStartTerminalsFlag()){
            fw.addStartTerminal(tok);
        }
        //cout<<"in expression(), peeked tok is non terminal, creating if header for nt\n";
        //fw.writeText(tok.lexeme, "ifHeader_nt");
        term();
        //cout<<"returned from term(), writing endif (in non t section)\n";
        //fw.writeText("", "endif");
    }
    else{
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
        //cout<<"ENTERING TERM()\n";
    factor();
    //cout<<"returned from factor()\n";
    Token tok = t.peekNextToken();
    while ((tok.type == "symbol" && (tok.lexeme == "{" || tok.lexeme == "[" || tok.lexeme == "(")) || tok.type == "terminal" || tok.type == "nonTerminal"){
        cout<<"peeked token: "+tok.lexeme+" "+tok.type+"\n";
        factor();
        //cout<<"returned from factor()\n";
        tok = t.peekNextToken();
        //cout<<"in term(), next token is "+tok.lexeme+"\n";
    }
}

void factor(){
        //cout<<"ENTERING FACTOR()\n";
    Token tok = t.getNextToken();
    if (tok.type == "terminal" || tok.type == "nonTerminal"){
        if (tok.type == "nonTerminal"){
            //cout<<"in factor, seen token" + tok.lexeme + "\n";
            fw.writeText(tok.lexeme, "nt");
        }
        else{
            cout<<"in factor, seen token" + tok.lexeme + "\n";
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
        //cout<<"in factor, seen token" + tok.lexeme + " calling expression()...\n";
        expression();
        tok = t.getNextToken();
        if (tok.type == "symbol" && tok.lexeme == "}"){
            //cout<<"in factor, seen token" + tok.lexeme + " writing zeroor many end\n";
            fw.writeText("", "zeroormany_end");
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
        fw.writeText("", "bracketSeen");
        //cout<<"in factor, seen token" + tok.lexeme + " calling expression()...\n";
        expression();
        tok = t.getNextToken();
        if (tok.type == "symbol" && tok.lexeme == "]"){
            //cout<<"in factor, seen token" + tok.lexeme + " writing zeroor one end\n";
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


