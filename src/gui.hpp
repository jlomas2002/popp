#ifndef GUI_HPP
#define GUI_HPP

#include <QtWidgets>

class GUI : public QMainWindow
{
    Q_OBJECT
public:
    GUI(QWidget *parent = nullptr);

private:
    QString projectName;
    QString chosenLang;

    QWidget *centralWidget;
    QVBoxLayout *mainLayout;

    QLabel *optionsLabel;
    QLabel *projectLabel;
    QLineEdit *projectNameInput;
    QLabel *fileUploadLabel;
    QPushButton *gramFileSelect_button;
    QPushButton *tokFileSelect_button;
    QLabel *languageOptionsLabel;
    QButtonGroup *languageOptions;
    QRadioButton *optionJava;
    QRadioButton *optionCpp;
    QRadioButton *optionPy;
    QPushButton *createParser_button;
    QWidget *optionsWidget;

    QLabel *inputLabel;
    QLabel *grammarLabel;
    QPushButton *grammarHelp_button;
    QPushButton *saveGrammar_button;
    QTextEdit *grammarDisplay;
    QLabel *tokensLabel;
    QPushButton *tokensHelp_button;
    QPushButton *saveTokens_button;
    QTextEdit *tokensDisplay;
    QTextBrowser *errorDisplay;
    QWidget *inputWidget;

    QLabel *outputLabel;
    QTextEdit *parserDisplay;
    QTextEdit *lexerDisplay;
    QTextEdit *tokDefDisplay;
    QTextEdit *parserHeaderDisplay;
    QTextEdit *lexerHeaderDisplay;
    QTabWidget *outputFilesTabbed;
    QTextEdit *testString;
    QPushButton *runTestString_button;
    QTextBrowser *testStringOutput;
    QPushButton *saveOutputFile_button;
    QWidget *outputWidget;


public slots:
    void selectGrammarFile();
    void selectTokensFile();
    void saveGrammarFile();
    void displayGrammarHelp();
    void saveTokensFile();
    void displayTokensHelp();
    void saveOutputFile();
    void createParser();
    void runTestString();
};

#endif // GUI_HPP

