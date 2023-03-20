#include <iostream>
#include <fstream>
#include <QCoreApplication>
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

    languageOptions = new QButtonGroup();
    optionCpp = new QRadioButton("C++");
    optionPy = new QRadioButton("Python");
    optionCpp->setChecked(true);

    languageOptions->addButton(optionCpp);
    languageOptions->addButton(optionPy);

    parserName = new QLineEdit();

    // Set up the layout
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(parserName);
    layout->addWidget(optionCpp);
    layout->addWidget(optionPy);
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
    if (grammarDisplay->toPlainText().trimmed() == "" || parserName->text().trimmed() == ""){
        //error
        return;
    }
    string grammar = (grammarDisplay->toPlainText()).toStdString();
    grammar.erase(grammar.size() - 1); //remove uneeded terminal char

    string parserFilename = parserName->text().toStdString();

    string chosenLang;
    QString pfile_extension;
    if (languageOptions->checkedButton() == optionCpp){
        chosenLang = "c++";
        pfile_extension = QString::fromStdString(parserFilename) + ".cpp";
    }
    else if (languageOptions->checkedButton() == optionPy){
        chosenLang = "python";
        pfile_extension = QString::fromStdString(parserFilename) + ".py";
    }

    Tokeniser tokeniser(grammar);
    FileWriter fileWriter(pfile_extension.toStdString(), chosenLang);
    parseGrammar(tokeniser, fileWriter);

    QFile parserFile(pfile_extension);
    if (!parserFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        //error message
        return;
    }

    QString parser = QString(parserFile.readAll());

    parserDisplay->setPlainText(parser);
}
