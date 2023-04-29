#include <iostream>
#include <cctype>
#include "gparser.hpp"
using namespace std;

Tokeniser *t = new Tokeniser("", "");
FileWriter *fw = new FileWriter("");
//Flag required so to of the same terminal checks aren't written to the file
bool firstTerminal;
bool veryFirstElement;

string startFunc;

int bracketTracker = 0;

Gtoken parseGrammar(Tokeniser &tokeniser, FileWriter &writer){
    t = &tokeniser;
    fw = &writer;
    firstTerminal = true;
    veryFirstElement = true;
    startFunc = "";

    Gtoken tok = grammar();
    fw->createMain(startFunc);
    return tok;
}

Gtoken grammar(){
    Gtoken tok = t->peekNextToken();
    while (tok.type == Non_Terminal){
        if (startFunc == ""){
            startFunc = tok.lexeme;
        }

        fw->writeText(tok.lexeme, Func_Begin);
        t->getNextToken();
        tok = rule();
        if (tok.type == Error_Type) return tok;
        firstTerminal = true;
        tok = t->peekNextToken();
    }

    if (tok.type != END_OF_GRAMMAR){
         //ERROR
        if (tok.error ==  ExpectedSemiColon || tok.error == UnknownCharacter){
            return tok;
        }
        return t->makeErrorToken(ExpectedNonTerminal, tok.lexeme);
    }

     return tok;
}

Gtoken rule(){
    Gtoken tok = t->getNextToken();
    if (tok.type == Error_Type) return tok;

    bracketTracker = 0;
    if (tok.lexeme == "=" && tok.type == Symbol){
        veryFirstElement = true;
        tok = expression();
        if (tok.type == Error_Type) return tok;
    }
    else{
        //ERROR
        return t->makeErrorToken(ExpectedEquals, tok.lexeme);
    }

    tok = t->getNextToken();
    if(tok.lexeme == ";" && tok.type == Symbol){
        fw->writeText("", Func_End);
    }
    else{
        //ERROR
        return t->makeErrorToken(ExpectedSemiColon, tok.lexeme);
    }

    return tok;
}

Gtoken expression(){
    firstTerminal = true;
    Gtoken tok;

    tok = t->peekNextToken();

    if (tok.type == Terminal || tok.type == Non_Terminal || tok.type == Token_Type){
        fw->addFirstElement(tok);

        if (veryFirstElement){
            fw->writeText("", Peek_NextTok);
            veryFirstElement = false;
        }

        if (tok.type == Terminal){
            fw->writeText(tok.lexeme, If_Begin_Terminal);
            fw->writeText("", Get_NextTok_Commented); //To consume the peeked token
        }

        else if (tok.type == Token_Type){
            fw->writeText(tok.lexeme, If_Begin_Token);
            fw->writeText("", Get_NextTok_Commented); //To consume the peeked token
        }

        else if (tok.type == Non_Terminal){
            firstTerminal = false;
            fw->writeText(tok.lexeme, If_Begin_NonTerminal);
        }

        tok = term();
        if (tok.type == Error_Type) return tok;
        fw->writeText("", Scope_End);
    }

    else{
        veryFirstElement = false;
        tok = term();
        if (tok.type == Error_Type) return tok;
    }

    tok = t->peekNextToken();
    while (tok.lexeme == "|" && tok.type == Symbol){
        t->getNextToken();

        tok = t->peekNextToken();
        if (tok.type == Terminal || tok.type == Non_Terminal || tok.type == Token_Type){
            fw->addFirstElement(tok);

            if (tok.type == Terminal){
                firstTerminal = true;
                
                fw->writeText(tok.lexeme, ElseIf_Terminal);
                fw->writeText("", Get_NextTok_Commented); //To consume the peeked token
            }

            else if (tok.type == Token_Type){
                firstTerminal = true;

                fw->writeText(tok.lexeme, ElseIf_Token);
                fw->writeText("", Get_NextTok_Commented); //To consume the peeked token
            }

            else if (tok.type == Non_Terminal){
                firstTerminal = false;
                fw->writeText(tok.lexeme, ElseIf_NonTerminal);
            }

            tok = term();
            if (tok.type == Error_Type) return tok;
            fw->writeText("", Scope_End);
        }
        else{
            tok = term();
            if (tok.type == Error_Type) return tok;
        }

        tok = t->peekNextToken();
    }

    if (!bracketTracker){
        fw->writeText("", Else_Expr);
        fw->writeText("", Scope_End);
    }

    return tok;

}

Gtoken term(){
    Gtoken tok;
    tok = factor();
    if (tok.type == Error_Type) return tok;
    tok = t->peekNextToken();
    while ((tok.type == Symbol && (tok.lexeme == "{" || tok.lexeme == "[" || tok.lexeme == "(")) || tok.type == Terminal || tok.type == Non_Terminal || tok.type == Token_Type){
        tok = factor();
        if (tok.type == Error_Type) return tok;
        tok = t->peekNextToken();
    }

    return tok;
}

Gtoken factor(){
    Gtoken tok = t->getNextToken();

    if (tok.type == Terminal || tok.type == Token_Type){
        if (firstTerminal){
            firstTerminal = false;
        }

        else{
            fw->writeText("", Get_NextTok);

            if (tok.type == Terminal){
                fw->writeText(tok.lexeme, If_Begin_Terminal);
                fw->writeText("", TerminalCheck_End);
                fw->writeText(tok.lexeme, Else_Fact_Terminal);
            }
            else{
                fw->writeText(tok.lexeme, If_Begin_Token);
                fw->writeText("", TerminalCheck_End);
                fw->writeText(tok.lexeme, Else_Fact_Token);
            }
            fw->writeText("", Scope_End);
        }
    }

    else if (tok.type == Non_Terminal){
        fw->writeText(tok.lexeme, NonTerminal_Seen);
    }

    else if(tok.type == Symbol && tok.lexeme == "{"){
        bracketTracker++;

        fw->writeText("", Bracket_Begin);
        tok = expression();
        if (tok.type == Error_Type) return tok;
        
        tok = t->getNextToken();

        if (tok.type == Symbol && tok.lexeme == "}"){
            fw->writeText("", CurlyBracket_End);

            bracketTracker--;
        }

        else{
            //ERROR
            return t->makeErrorToken(ExpectedCurlyBracket, tok.lexeme);
        } 
    } 

    else if(tok.type == Symbol && tok.lexeme == "("){
        bracketTracker++;

        fw->writeText("", Peek_NextTok);
        tok = expression();
        if (tok.type == Error_Type) return tok;

        tok = t->getNextToken();

        if (tok.type == Symbol && tok.lexeme == ")"){
            bracketTracker--;
        }

        else{
            //ERROR
            return t->makeErrorToken(ExpectedNormalBracket, tok.lexeme);
        } 
    }

    else if(tok.type == Symbol && tok.lexeme == "["){
        bracketTracker++;
        fw->writeText("", Bracket_Begin);
        tok = expression();
        if (tok.type == Error_Type) return tok;

        tok = t->getNextToken();

        if (tok.type == Symbol && tok.lexeme == "]"){
            fw->writeText("", SquareBracket_End);

            bracketTracker--;
        }

        else{
            //ERROR
            return t->makeErrorToken(ExpectedSquareBracket, tok.lexeme);
        } 
    }
    else if (tok.type == Error_Type){
        //ERROR
        return tok;
    }

    return tok;

}
