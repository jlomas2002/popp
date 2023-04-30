#ifndef GUI_HPP
#define GUI_HPP

#include <QtWidgets>


/**
 * @brief Handles GUI
 * 
 */
class GUI : public QMainWindow
{
    Q_OBJECT
public:
    GUI(QWidget *parent = nullptr);

private:
    QString projectName;
    QString chosenLang;

    QString grammarHelp;
    QString tokensHelp;

    QWidget *centralWidget;
    QVBoxLayout *mainLayout;

    //Widgets for control panel
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

    //Widgets for input section
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

    //Widgets for output section
    QLabel *outputLabel;
    QTextEdit *parserDisplay;
    QTextEdit *lexerDisplay;
    QTextEdit *tokDefDisplay;
    QTextEdit *parserHeaderDisplay;
    QTextEdit *lexerHeaderDisplay;
    QTabWidget *outputFilesTabbed;
    QLabel *testStringLabel;
    QTextEdit *testString;
    QPushButton *runTestString_button;
    QTextBrowser *testStringOutput;
    QPushButton *saveOutputFile_button;
    QWidget *outputWidget;


public slots:
    /**
     * @brief Allows user to select grammar file from file explorer
     * 
     */
    void selectGrammarFile();
    /**
     * @brief Allows user to select tokens file from file explorer
     * 
     */
    void selectTokensFile();
    /**
     * @brief Allows user to save the text in the grammar display to a file
     * 
     */
    void saveGrammarFile();
    void displayGrammarHelp();
    /**
     * @brief Allows user to save the text in the tokens display to a file
     * 
     */
    void saveTokensFile();
    void displayTokensHelp();
    /**
     * @brief Allows user to update an ouput file with their changes
     * 
     */
    void saveOutputFile();
    /**
     * @brief Generates all output files from grammar
     * 
     */
    void createParser();
    /**
     * @brief Compiles and executes the generated files and passes in the test string to see if it is valid
     * 
     */
    void runTestString();
};

#endif // GUI_HPP

