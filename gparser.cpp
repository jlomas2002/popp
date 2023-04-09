#include <iostream>
#include <cctype>
#include "gparser.hpp"
using namespace std;

Tokeniser t("", "");
FileWriter fw("", "");
//Flag required so to of the same terminal checks aren't written to the file
bool startTerminal = true;
bool veryFirstElement = true;

void parseGrammar(Tokeniser &tokeniser, FileWriter writer){
    t = tokeniser;
    fw = writer;
    grammar();
}

void grammar(){
    Gtoken tok = t.peekNextToken();
    while ((tok.type == Non_Terminal)){
        fw.writeText(tok.lexeme, Func_Begin);
        t.getNextToken();
        rule();
        startTerminal = true;
        tok = t.peekNextToken();
    }

    if (tok.type != END_OF_GRAMMAR){
         //ERROR
         tok = t.makeErrorToken(ExpectedNonTerminal);
     }
}

void rule(){
    Gtoken tok = t.getNextToken();
    if (tok.lexeme == "=" && tok.type == Symbol){
        veryFirstElement = true;
        expression();
    }
    else{
        //ERROR
        tok = t.makeErrorToken(ExpectedEquals);
    }

    tok = t.getNextToken();
    if(tok.lexeme == ";" && tok.type == Symbol){
        fw.writeText("", Func_End);
    }
    else{
        //ERROR
        tok = t.makeErrorToken(ExpectedSemiColon);
    }
}

void expression(){
    startTerminal = true;
    Gtoken tok;

    tok = t.peekNextToken();

    if (tok.type == Terminal || tok.type == Non_Terminal || tok.type == Token_Type){
        fw.addStartTerminal(tok);

        if (veryFirstElement){
            fw.writeText("", Peek_NextTok);
            veryFirstElement = false;
        }

        if (tok.type == Terminal){
            fw.writeText(tok.lexeme, If_Begin_Terminal);
            fw.writeText("", Get_NextTok); //To consume the peeked token
        }

        else if (tok.type == Token_Type){
            fw.writeText(tok.lexeme, If_Begin_Token);
            fw.writeText("", Get_NextTok); //To consume the peeked token
        }

        else if (tok.type == Non_Terminal){
            startTerminal = false;
            fw.writeText(tok.lexeme, If_Begin_NonTerminal);
        }

        term();
        fw.writeText("", Scope_End);
    }

    else{
        veryFirstElement = false;
        term();
    }

    tok = t.peekNextToken();
    while (tok.lexeme == "|" && tok.type == Symbol){
        t.getNextToken();

        tok = t.peekNextToken();
        if (tok.type == Terminal || tok.type == Non_Terminal || tok.type == Token_Type){
            fw.addStartTerminal(tok);

            if (tok.type == Terminal){
                startTerminal = true;
                
                fw.writeText(tok.lexeme, ElseIf_Terminal);
                fw.writeText("", Get_NextTok); //To consume the peeked token
            }

            else if (tok.type == Token_Type){
                startTerminal = true;

                fw.writeText(tok.lexeme, ElseIf_Token);
                fw.writeText("", Get_NextTok); //To consume the peeked token
            }

            else if (tok.type == Non_Terminal){
                startTerminal = false;
                fw.writeText(tok.lexeme, ElseIf_NonTerminal);
            }

            term();
            fw.writeText("", Scope_End);
        }
        else{
            term();
        }

        tok = t.peekNextToken();
    }

    // if (!insideBracket){
    //     fw.writeText("", Else_Expr);
    //     fw.writeText("", Scope_End);
    // }

}

void term(){
    factor();
    Gtoken tok = t.peekNextToken();
    while ((tok.type == Symbol && (tok.lexeme == "{" || tok.lexeme == "[" || tok.lexeme == "(")) || tok.type == Terminal || tok.type == Non_Terminal || tok.type == Token_Type){
        factor();
        tok = t.peekNextToken();
    }
}

void factor(){
    Gtoken tok = t.getNextToken();

    if (tok.type == Terminal  || tok.type == Token_Type){
        if (startTerminal){
            startTerminal = false;
        }

        else{
            fw.writeText("", Get_NextTok);

            if (tok.type == Terminal){
                fw.writeText(tok.lexeme, If_Begin_Terminal);
                fw.writeText("", Scope_End);
                fw.writeText(tok.lexeme, Else_Fact_Terminal);
            }
            else{
                fw.writeText(tok.lexeme, If_Begin_Token);
                fw.writeText("", Scope_End);
                fw.writeText(tok.lexeme, Else_Fact_Token);
            }
            fw.writeText("", Scope_End);
        }
    }

    else if (tok.type == Non_Terminal){
        fw.writeText(tok.lexeme, NonTerminal_Seen);
    }

    else if(tok.type == Symbol && tok.lexeme == "{"){
        fw.writeText("", Bracket_Begin);
        expression();
        tok = t.getNextToken();

        if (tok.type == Symbol && tok.lexeme == "}"){
            cout<<"here200\n";
            fw.writeText("", CurlyBracket_End);
        }

        else{
            //ERROR
            tok = t.makeErrorToken(ExpectedCurlyBracket);     
        } 
    } 

    else if(tok.type == Symbol && tok.lexeme == "("){
        fw.writeText("", Peek_NextTok);
        expression();
        tok = t.getNextToken();

        if (tok.type == Symbol && tok.lexeme == ")"){
            ;//Good
        }

        else{
            //ERROR
            tok = t.makeErrorToken(ExpectedNormalBracket); 
        } 
    }

    else if(tok.type == Symbol && tok.lexeme == "["){
        fw.writeText("", Bracket_Begin);
        expression();
        tok = t.getNextToken();

        if (tok.type == Symbol && tok.lexeme == "]"){
            cout<<"HERE\n";
            fw.writeText("", SquareBracket_End);
        }

        else{
            //ERROR
            tok = t.makeErrorToken(ExpectedSquareBracket); 
        } 
    }
    else{
        //ERROR
        tok = t.makeErrorToken(SyntacticError); 
    } 

}


