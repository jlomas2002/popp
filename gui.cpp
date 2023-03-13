#include <iostream>
#include <fstream>
#include "gui.hpp"
#include "tokeniser.hpp"
#include "gparser.hpp"
#include "fileWriter.hpp"

using namespace std;


GUI::GUI(QWidget *parent) : QWidget(parent){
    QMainWindow window;

    gFileSelect_button = new QPushButton("Select a grammar file.");
    connect(gFileSelect_button, &QPushButton::clicked, this, &GUI::selectGrammarFile);

    createParser_button = new QPushButton("Generate parser.");
    connect(createParser_button, &QPushButton::clicked, this, &GUI::createParser);

    grammarDisplay = new QTextEdit();
    grammarDisplay->setPlainText("Type in your grammar...");

    parserDisplay = new QTextBrowser();

    // Set up the layout
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(gFileSelect_button);
    layout->addWidget(createParser_button);
    layout->addWidget(grammarDisplay);
    layout->addWidget(parserDisplay);
    setLayout(layout);
}

void GUI::selectGrammarFile(){
    QString fileName = QFileDialog::getOpenFileName(nullptr, "Select file", "", "All Files (*.txt)");
    if (!fileName.isEmpty()) {
      ifstream fileReader(fileName.toStdString());
      if (!fileReader.is_open()){
            //error message
      }

      string rule;
      string grammar;

      while(getline (fileReader, rule)){
           grammar.append(rule);
           grammar.append("\n");
      }
      fileReader.close();

      grammarDisplay->setPlainText(QString::fromStdString(grammar));
    }
}

void GUI::createParser(){
    string grammar = (grammarDisplay->toPlainText()).toStdString();
    grammar.erase(grammar.size() - 1); //remove uneeded terminal char

    qDebug()<<"here1\n";
    Tokeniser tokeniser(grammar);
    qDebug()<<"here2\n";
    FileWriter fileWriter("test.txt");
    qDebug()<<"here3\n";
    parseGrammar(tokeniser, fileWriter);
    qDebug()<<"here4\n";

    QFile parserFile("test.txt");
    if (!parserFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        //error message
    }

    QString parser = QString(parserFile.readAll());

    parserDisplay->setPlainText(parser);
}
