#include <iostream>
#include <fstream>
#include <QCoreApplication>
#include <Windows.h>
#include "gui.hpp"
#include "tokeniser.hpp"
#include "gparser.hpp"
#include "fileWriter.hpp"

using namespace std;

GUI::GUI(QWidget *parent) : QMainWindow(parent){
    QHBoxLayout *mainLayout = new QHBoxLayout;

    //Options sections
    QVBoxLayout *optionsSection = new QVBoxLayout;

    optionsLabel = new QLabel();
    optionsLabel->setText("Control Panel");

    //Name options
    QVBoxLayout *projectNameOptions = new QVBoxLayout;

    projectLabel = new QLabel();
    projectLabel->setText("Name your project");

    projectNameInput = new QLineEdit();
    projectNameInput->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

    projectNameOptions->addWidget(projectLabel);
    projectNameOptions->addWidget(projectNameInput);

    //File upload options
    QVBoxLayout *fileSelectOptions = new QVBoxLayout;

    fileUploadLabel = new QLabel();
    fileUploadLabel->setText("Upload files");

    gramFileSelect_button = new QPushButton("Select a grammar file.");
    connect(gramFileSelect_button, &QPushButton::clicked, this, &GUI::selectGrammarFile);
    gramFileSelect_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

    tokFileSelect_button = new QPushButton("Select a token file.");
    connect(tokFileSelect_button, &QPushButton::clicked, this, &GUI::selectTokensFile);
    tokFileSelect_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

    fileSelectOptions->addWidget(fileUploadLabel);
    fileSelectOptions->addWidget(gramFileSelect_button);
    fileSelectOptions->addWidget(tokFileSelect_button);

    //Language options
    QVBoxLayout *languageSelectOptions = new QVBoxLayout;

    languageOptionsLabel = new QLabel();
    languageOptionsLabel->setText("Select a target language");

    languageOptions = new QButtonGroup();

    optionCpp = new QRadioButton("C++");
    optionCpp->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

    optionPy = new QRadioButton("Python");
    optionPy->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

    optionJava = new QRadioButton("Java");
    optionJava->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

    optionCpp->setChecked(true);
    languageOptions->addButton(optionCpp);
    languageOptions->addButton(optionPy);
    languageOptions->addButton(optionJava);

    languageSelectOptions->addWidget(languageOptionsLabel);
    languageSelectOptions->addWidget(optionCpp);
    languageSelectOptions->addWidget(optionPy);
    languageSelectOptions->addWidget(optionJava);

    //Create parser option
    QVBoxLayout *createParserOptions = new QVBoxLayout;

    createParser_button = new QPushButton("Generate parser.");
    connect(createParser_button, &QPushButton::clicked, this, &GUI::createParser);
    createParser_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

    createParserOptions->addWidget(createParser_button);

    //Widget size settings
    //widths
    optionsLabel->setFixedWidth(75);
    projectNameInput->setFixedWidth(75);
    gramFileSelect_button->setFixedWidth(75);
    tokFileSelect_button->setFixedWidth(75);
    optionCpp->setFixedWidth(75);
    optionPy->setFixedWidth(75);
    createParser_button->setFixedWidth(75);

    //heights
    optionsLabel->setFixedHeight(25);
    projectNameInput->setFixedHeight(25);
    gramFileSelect_button->setFixedHeight(25);
    tokFileSelect_button->setFixedHeight(25);
    optionCpp->setFixedHeight(25);
    optionPy->setFixedHeight(25);
    createParser_button->setFixedHeight(25);

    //Add subsections to section
    optionsSection->addWidget(optionsLabel);
    optionsSection->addLayout(projectNameOptions);
    optionsSection->addLayout(fileSelectOptions);
    optionsSection->addLayout(languageSelectOptions);
    optionsSection->addLayout(createParserOptions);

    //Set colour
    optionsWidget = new QWidget;
    optionsWidget->setLayout(optionsSection);
    optionsWidget->setStyleSheet("background-color: rgb(77, 70, 87);");
    optionsWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    optionsWidget->setFixedWidth(150);

    //Input files section
    QVBoxLayout *inputFilesSection = new QVBoxLayout;

    inputLabel = new QLabel();
    inputLabel->setText("Input files");

    QHBoxLayout *grammarHeader = new QHBoxLayout;
    grammarLabel = new QLabel();
    grammarLabel->setText("Enter Grammar");

    //Save Grammar button
    saveGrammar_button = new QPushButton;
    connect(saveGrammar_button, &QPushButton::clicked, this, &GUI::saveGrammarFile);
    //Add icon for button
    QIcon saveAsIcon("C:/Users/Jloma/Documents/UniYr3/parcelona/qtIcons/saveFileAs.png");
    saveGrammar_button->setIcon(saveAsIcon);
    saveGrammar_button->setIconSize(QSize(16, 16));
    saveGrammar_button->setToolTip("Save grammar as");
    saveGrammar_button->setFixedWidth(30);

    //Grammar help button
    grammarHelp_button = new QPushButton();
    connect(grammarHelp_button, &QPushButton::clicked, this, &GUI::displayGrammarHelp);
    //Add icon for button
    QIcon helpIcon("C:/Users/Jloma/Documents/UniYr3/parcelona/qtIcons/help.png");
    grammarHelp_button->setIcon(helpIcon);
    grammarHelp_button->setIconSize(QSize(16, 16));
    grammarHelp_button->setToolTip("Click me for help!");
    grammarHelp_button->setFixedWidth(30);

    grammarHeader->addWidget(grammarLabel);
    grammarHeader->addWidget(saveGrammar_button);
    grammarHeader->addWidget(grammarHelp_button);

    //Grammar display
    grammarDisplay = new QTextEdit();
    grammarDisplay->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    grammarDisplay->setStyleSheet("background-color: rgb(125, 118, 100);");

    QHBoxLayout *tokensHeader = new QHBoxLayout;
    tokensLabel = new QLabel();
    tokensLabel->setText("(optional) Enter Tokens");

    //Save Grammar button
    saveTokens_button = new QPushButton;
    connect(saveTokens_button, &QPushButton::clicked, this, &GUI::saveTokensFile);
    //Add icon for button
    saveTokens_button->setIcon(saveAsIcon);
    saveTokens_button->setIconSize(QSize(16, 16));
    saveTokens_button->setToolTip("Save tokens as");
    saveTokens_button->setFixedWidth(30);

    //Grammar help button
    tokensHelp_button = new QPushButton();
    connect(tokensHelp_button, &QPushButton::clicked, this, &GUI::displayTokensHelp);
    //Add icon for button
    tokensHelp_button->setIcon(helpIcon);
    tokensHelp_button->setIconSize(QSize(16, 16));
    tokensHelp_button->setToolTip("Click me for help!");
    tokensHelp_button->setFixedWidth(30);

    tokensHeader->addWidget(tokensLabel);
    tokensHeader->addWidget(saveTokens_button);
    tokensHeader->addWidget(tokensHelp_button);

    //Tokens display
    tokensDisplay = new QTextEdit();
    tokensDisplay->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    //Error display
    errorDisplay = new QTextBrowser();
    errorDisplay->setPlainText("No errors!");
    errorDisplay->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    //Set sizes of widgets
    grammarDisplay->setMinimumHeight(400);
    tokensDisplay->setMinimumHeight(250);
    grammarDisplay->setMinimumWidth(90);
    tokensDisplay->setMinimumWidth(90);
    errorDisplay->setFixedHeight(40);

    inputFilesSection->addWidget(inputLabel);
    inputFilesSection->addLayout(grammarHeader);
    inputFilesSection->addWidget(grammarDisplay);
    inputFilesSection->addLayout(tokensHeader);
    inputFilesSection->addWidget(tokensDisplay);
    inputFilesSection->addWidget(errorDisplay);

    //Set colour
    inputWidget = new QWidget;
    inputWidget->setLayout(inputFilesSection);
    inputWidget->setStyleSheet("background-color: rgb(175, 152, 212);");

    //Output files section
    QVBoxLayout *outputFilesSection = new QVBoxLayout;

    outputLabel = new QLabel();
    outputLabel->setText("Output files");

    //Header for tabbed section
    QHBoxLayout *outputHeader = new QHBoxLayout();

    //Save file button
    saveOutputFile_button = new QPushButton();
    connect(saveOutputFile_button, &QPushButton::clicked, this, &GUI::saveOutputFile);
    //Add icon for button
    QIcon saveIcon("C:/Users/Jloma/Documents/UniYr3/parcelona/qtIcons/saveFile.png");
    saveOutputFile_button->setIcon(saveIcon);
    saveOutputFile_button->setIconSize(QSize(16, 16));
    saveOutputFile_button->setToolTip("Save file");
    saveOutputFile_button->setFixedWidth(30);

    outputHeader->addWidget(outputLabel);
    outputHeader->addWidget(saveOutputFile_button);

    //Parser display
    parserDisplay = new QTextEdit();
    parserDisplay->setPlainText("Your parser will appear here...");
    parserDisplay->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    //Lexer display
    lexerDisplay = new QTextEdit();
    lexerDisplay->setPlainText("Your lexer will appear here...");
    lexerDisplay->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    //Token defintion display
    tokDefDisplay = new QTextEdit();
    tokDefDisplay->setPlainText("Your token definition will appear here...");
    lexerDisplay->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    //Parser header display
    parserHeaderDisplay = new QTextEdit();
    parserHeaderDisplay->setPlainText("Your parser header will appear here if required...");
    parserHeaderDisplay->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    //Lexer header display
    lexerHeaderDisplay = new QTextEdit();
    lexerHeaderDisplay->setPlainText("Your parser header will appear here if required...");
    lexerHeaderDisplay->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);


    //Set sizes for displays
    parserDisplay->setMinimumHeight(600);
    lexerDisplay->setMinimumHeight(600);
    tokDefDisplay->setMinimumHeight(600);
    parserHeaderDisplay->setMinimumHeight(600);
    lexerHeaderDisplay->setMinimumHeight(600);

    //Test string input and run button
    QHBoxLayout *stringTestingBit = new QHBoxLayout;
    testString = new QTextEdit();
    testString->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    //Run string test button
    runTestString_button = new QPushButton();
    connect(runTestString_button, &QPushButton::clicked, this, &GUI::runTestString);
    runTestString_button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    //Add icon for run button
    QIcon playIcon("C:/Users/Jloma/Documents/UniYr3/parcelona/qtIcons/greenPlay.png");
    runTestString_button->setIcon(playIcon);
    runTestString_button->setIconSize(QSize(16, 16));
    runTestString_button->setToolTip("Parse this text");

    runTestString_button->setFixedWidth(20);

    stringTestingBit->addWidget(testString);
    stringTestingBit->addWidget(runTestString_button);

    //Test string output
    testStringOutput = new QTextBrowser();
    testStringOutput->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    //Make the parser and lexer display a tabbed layout
    outputFilesTabbed = new QTabWidget();
    outputFilesTabbed->addTab(parserDisplay, "Parser");
    outputFilesTabbed->addTab(lexerDisplay, "Lexer");
    outputFilesTabbed->addTab(tokDefDisplay, "Token Definition");
    outputFilesTabbed->addTab(parserHeaderDisplay, "Parser Header");
    outputFilesTabbed->addTab(lexerHeaderDisplay, "Lexer Header");

    parserDisplay->show();

    outputFilesSection->addLayout(outputHeader);
    outputFilesSection->addWidget(outputFilesTabbed);
    outputFilesSection->addLayout(stringTestingBit);
    outputFilesSection->addWidget(testStringOutput);

    //Set colour
    outputWidget = new QWidget;
    outputWidget->setLayout(outputFilesSection);
    outputWidget->setStyleSheet("background-color: rgb(175, 152, 212);");

    //Make sections expandable
    inputFilesSection->addStretch(1);
    outputFilesSection->addStretch(1);

    QFrame* divider1 = new QFrame();
    divider1->setFrameShape(QFrame::VLine);
    divider1->setFrameShadow(QFrame::Sunken);
    divider1->setStyleSheet("background-color: rgb(98, 3, 253);");

    QFrame* divider2 = new QFrame();
    divider2->setFrameShape(QFrame::VLine);
    divider2->setFrameShadow(QFrame::Sunken);
    divider2->setStyleSheet("background-color: rgb(98, 3, 253);");

    //Add sections to main layout
    mainLayout->setSpacing(0);
    mainLayout->addWidget(optionsWidget);
    mainLayout->addWidget(divider1);
    mainLayout->addWidget(inputWidget);
    mainLayout->addWidget(divider2);
    mainLayout->addWidget(outputWidget);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setStyleSheet("background-color: rgb(110, 100, 125);");
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
}

void GUI::selectGrammarFile(){
    QString fileName = QFileDialog::getOpenFileName(nullptr, "Select file", "", "All Files (*.txt)");
    if (!fileName.isEmpty()) {
      ifstream fileReader(fileName.toStdString());
      if (!fileReader.is_open()){
          QMessageBox::information(this, "Error", "Couldn't open file.");
          return;
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

void GUI::selectTokensFile(){
    QString fileName = QFileDialog::getOpenFileName(nullptr, "Select file", "", "All Files (*.txt)");
    if (!fileName.isEmpty()) {
      ifstream fileReader(fileName.toStdString());
      if (!fileReader.is_open()){
          QMessageBox::information(this, "Error", "Couldn't open file.");
          return;
      }

      string line;
      string tokens;

      while(getline (fileReader, line)){
           tokens.append(line);
           tokens.append("\n");
      }
      fileReader.close();

      tokensDisplay->setPlainText(QString::fromStdString(tokens));
    }
}

void GUI::saveGrammarFile(){
    QString filename = QFileDialog::getSaveFileName(this,tr("Save grammar file"), "/home/user/Documents", tr("All files (*.*)"));

    if (!filename.isEmpty()) {
        ofstream grammarFile;
        grammarFile.open(filename.toStdString(), ios_base::out);
        grammarFile<<grammarDisplay->toPlainText().toStdString();
        grammarFile.close();
    }
}

void GUI::saveTokensFile(){
    QString filename = QFileDialog::getSaveFileName(this,tr("Save tokens file"), "/home/user/Documents", tr("All files (*.*)"));

    if (!filename.isEmpty()) {
        ofstream tokenFile;
        tokenFile.open(filename.toStdString(), ios_base::out);
        tokenFile<<grammarDisplay->toPlainText().toStdString();
        tokenFile.close();
    }
}

void GUI::displayGrammarHelp(){
    QMessageBox info;
    info.setWindowTitle("Grammar help");
    info.setText("dshskhfkjshkh");
    info.setIcon(QMessageBox::Information);
    info.exec();
}

void GUI::displayTokensHelp(){
    QMessageBox info;
    info.setWindowTitle("Tokens help");
    info.setText("");
    info.setIcon(QMessageBox::Information);
    info.exec();
}

void GUI::saveOutputFile(){

    //Save the file if a project name exists
    if (projectName.trimmed() == ""){
        QMessageBox::information(this, "Error.", "Project name is empty.");
        return;
    }

    //Get currently tabbed
    QWidget* currentWidget = outputFilesTabbed->currentWidget();
    QTextEdit* display = dynamic_cast<QTextEdit*>(currentWidget);

    QString filename;

    if (display == parserDisplay){
        filename = projectName + "Parser." + chosenLang;
    }
    else if (display == lexerDisplay){
        filename = projectName + "Lexer." + chosenLang;
    }
    else if (display == tokDefDisplay){
        if (chosenLang == "cpp"){
            filename = projectName + "Parser.hpp";
        }
        else{
            filename = projectName + "TokDefinition." + chosenLang;
        }
    }
    else if (display == parserHeaderDisplay){
        if (chosenLang == "cpp"){
            filename = projectName + "Parser.hpp";
        }
        else{
            QMessageBox::information(this, "Error.", "File does not exist.");
            return;
        }
    }
    else if (display == lexerHeaderDisplay){
        if (chosenLang == "cpp"){
            filename = projectName + "Lexer.hpp";
        }
        else{
            QMessageBox::information(this, "Error.", "File does not exist.");
            return;
        }
    }


    //If file already exists, this will update it
    //If file has been deleted, this will create a new one
    ofstream file;

    file.open(filename.toStdString(), ios_base::out);
    if (file.fail()){
        QMessageBox::information(this, "Error.", "Error writing to file.");
        return;
    }
    file<<display->toPlainText().toStdString();
    file.close();

    //when saved, output message saying file has been saved
    //(saved, not saved as)
    QMessageBox::information(this, "Success.", "File saved.");
    return;
}

void GUI::createParser(){
    errorDisplay->clear();
    parserDisplay->clear();
    lexerDisplay->clear();
    tokDefDisplay->clear();
    parserHeaderDisplay->clear();
    lexerHeaderDisplay->clear();
    chosenLang = "";

    if (projectNameInput->text().trimmed() == ""){
        QMessageBox::information(this, "Parser not generated.", "Please type in a project name before generating a parser.");
        return;
    }
    if (grammarDisplay->toPlainText().trimmed() == ""){
        QMessageBox::information(this, "Parser not generated.", "Please type in a grammar before generating a parser.");
        return;
    }

    string grammar = (grammarDisplay->toPlainText()).toStdString();
    grammar.erase(grammar.size() - 1); //remove uneeded terminal char

    projectName = projectNameInput->text();

    QString parserFilename;
    QString lexerFilename;
    QString tokDefFilename;
    QString parserHeaderFilename;
    QString lexerHeaderFilename;

    if (languageOptions->checkedButton() == optionCpp){
        chosenLang = "cpp";
        parserFilename = projectName + "Parser.cpp";
        lexerFilename = projectName + "Lexer.cpp";
        tokDefFilename = projectName + "TokDefinition.hpp";
        parserHeaderFilename = projectName + "Parser.hpp";
        lexerHeaderFilename = projectName + "Lexer.hpp";
    }
    else if (languageOptions->checkedButton() == optionPy){
        chosenLang = "py";
        parserFilename = projectName + "Parser.py";
        lexerFilename = projectName + "Lexer.py";
        tokDefFilename = projectName + "TokDefinition.py";
    }
    else if (languageOptions->checkedButton() == optionJava){
        chosenLang = "java";
        parserFilename = projectName + "Parser.java";
        lexerFilename = projectName + "Lexer.java";
        tokDefFilename = projectName + "TokDefinition.java";
    }

    string tokens = "";
    if (tokensDisplay->toPlainText().trimmed() != ""){
        tokens = tokensDisplay->toPlainText().toStdString();
    }

    Tokeniser tokeniser(grammar, tokens);
    if (tokeniser.getErrorState()){
        string errorFeedback = "";
        if (tokeniser.getGrammarFileError().type == Error_Type){
            Gtoken errorTok = tokeniser.getGrammarFileError();
            errorFeedback += "Error detected in grammar file.\n";
            errorFeedback += tokeniser.errorToString(errorTok.error);
            errorFeedback += ", found " + errorTok.lexeme + " in rule " + to_string(errorTok.lineNum) + " at position " + to_string(errorTok.pos--) + ".\n";
        }

        if(tokeniser.getTokenFileError().type == Error_Type){
            Gtoken errorTok = tokeniser.getTokenFileError();
            errorFeedback += "Error detected in tokens file.\n";
            errorFeedback += tokeniser.errorToString(errorTok.error);
            errorFeedback += ", found " + errorTok.lexeme + " in token definition " + to_string(errorTok.lineNum) + " at position " + to_string(errorTok.pos--) + ".\n";
        }
        errorFeedback += "See help buttons for how to define grammar or tokens.";
        errorDisplay->setText(QString::fromStdString(errorFeedback));
        return;
    }

    FileWriter fileWriter(chosenLang.toStdString());
    fileWriter.fileSetup(tokeniser.getAllFirstSetInfo(), tokeniser.getListOfTokens(), tokeniser.getTokenRegexes(), projectName.toStdString());
    Gtoken tok = parseGrammar(tokeniser, fileWriter);

    //If no error, output to file
    if (tok.type != Error_Type){
        ofstream parserFile;
        parserFile.open(parserFilename.toStdString(), ios_base::out);
        parserFile<<fileWriter.getParserText();
        parserFile.close();

        ofstream lexerFile;
        lexerFile.open(lexerFilename.toStdString(), ios_base::out);
        lexerFile<<fileWriter.getLexerText();
        lexerFile.close();

        ofstream tokDefFile;
        tokDefFile.open(tokDefFilename.toStdString(), ios_base::out);
        tokDefFile<<fileWriter.getTokDefText();
        tokDefFile.close();

        if (chosenLang == "cpp"){
            ofstream parserHeaderFile;
            parserHeaderFile.open(parserHeaderFilename.toStdString(), ios_base::out);
            parserHeaderFile<<fileWriter.getParserHeaderText();
            parserHeaderFile.close();

            ofstream lexerHeaderFile;
            lexerHeaderFile.open(lexerHeaderFilename.toStdString(), ios_base::out);
            lexerHeaderFile<<fileWriter.getLexerHeaderText();
            lexerHeaderFile.close();
        }
    }
    else{
        string errorFeedback = "";
        errorFeedback += "Error detected in grammar file.\n";
        errorFeedback += tokeniser.errorToString(tok.error);
        errorFeedback += ", found " + tok.lexeme + " in rule " + to_string(tok.lineNum) + " at position " + to_string(tok.pos--) + ".\n";

        errorDisplay->setText(QString::fromStdString(errorFeedback));
        return;
    }

    QString parserText = QString::fromStdString(fileWriter.getParserText());
    QString lexerText = QString::fromStdString(fileWriter.getLexerText());
    QString tokDefText = QString::fromStdString(fileWriter.getTokDefText());
    QString parserHeaderText = QString::fromStdString(fileWriter.getParserHeaderText());
    QString lexerHeaderText = QString::fromStdString(fileWriter.getLexerHeaderText());

    parserDisplay->setPlainText(parserText);
    lexerDisplay->setPlainText(lexerText);
    tokDefDisplay->setPlainText(tokDefText);
    parserHeaderDisplay->setPlainText(parserHeaderText);
    lexerHeaderDisplay->setPlainText(lexerHeaderText);
}

void GUI::runTestString(){
    testStringOutput->clear();

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    if (testString->toPlainText().trimmed() == ""){
        QMessageBox::information(this, "Error.", "Please enter some text.");
        return;
    }

    if (projectName.trimmed() == ""){
        QMessageBox::information(this, "Error.", "Project name unknown.");
        return;
    }

    if (chosenLang == ""){
        QMessageBox::information(this, "Error.", "You must choose a language and generate a parser");
        return;
    }

    //First write the test string to file as the parser/lexer requires a file input
    ofstream outFile;
    outFile.open(projectName.toStdString() + "TestString.txt", ios_base::out);
    outFile<<testString->toPlainText().toStdString();
    outFile.close();

    string compileCommand = "";
    string runCommand = "";
    string p = projectName.toStdString();
    if (chosenLang == "cpp"){
        compileCommand = "g++ " + p + "Parser.cpp " + p + "Parser.hpp " + p + "TokDefinition.hpp ";
        compileCommand += p + "Lexer.cpp " + p + "Lexer.hpp -o " + p;

        runCommand = "./" + p + " " + p + "TestString.txt";
    }
    else if (chosenLang == "py"){
        runCommand = "python " + p + "Parser.py";
    }
    else if (chosenLang == "java"){
        compileCommand = "javac " + p + "Parser.java " + p + "Lexer.java " + p + "TokDefinition.java ";

        runCommand = "java " + p + "Parser " + p + "TestString.txt";
    }

    //Compile
    if (chosenLang == "cpp" || chosenLang == "java"){
        if (!CreateProcess(NULL, const_cast<wchar_t*>(wstring(compileCommand.begin(), compileCommand.end()).c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
        {
            QMessageBox::information(this, "Error.", "Process could not be created.");
            return;
        }

        //Wait for compilation to finish
        DWORD result = WaitForSingleObject(pi.hProcess, 10000);
        if (result == WAIT_TIMEOUT){
            QMessageBox::information(this, "Error.", "Compilation timed out.");
            return;
        }

        DWORD exitCode;
        GetExitCodeProcess(pi.hProcess, &exitCode);
        if (exitCode != 0){
            QMessageBox::information(this, "Error.", "Compilation was unsuccessful.");
            return;
        }

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

    }

    //Run
    if (!CreateProcess(NULL, const_cast<wchar_t*>(wstring(runCommand.begin(), runCommand.end()).c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        QMessageBox::information(this, "Error.", "Process could not be created.");
        return;
    }

    //Wait for compilation to finish
    DWORD result = WaitForSingleObject(pi.hProcess, 10000);
    if (result == WAIT_TIMEOUT){
        QMessageBox::information(this, "Error.", "Running parser timed out.");
        return;
    }

    DWORD exitCode;
    GetExitCodeProcess(pi.hProcess, &exitCode);
    if (exitCode == 0){
        testStringOutput->setPlainText("Text is valid!");
    }
    else if (int(exitCode) == 2){
        testStringOutput->setPlainText("Text is not valid.");
    }
    else if (exitCode == 3){
        QMessageBox::information(this, "Error.", "Could not open file containing test string.");
        return;
    }
    else if (exitCode == 4){
        QMessageBox::information(this, "Error.", "Incorrect number of args passed to main.");
        return;
    }
    else{
        QMessageBox::information(this, "Error.", "Error when executing parser.");
        return;
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}
