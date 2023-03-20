#ifndef GUI_HPP
#define GUI_HPP

#include <QtWidgets>

class GUI : public QWidget
{
    Q_OBJECT
public:
    GUI(QWidget *parent = nullptr);

private:
    QPushButton *gFileSelect_button;
    QPushButton *createParser_button;
    QTextEdit *grammarDisplay;
    QTextBrowser *parserDisplay;
    QLineEdit *parserName;

    QButtonGroup *languageOptions;
    QRadioButton *optionCpp;
    QRadioButton *optionPy;

public slots:
    void selectGrammarFile();
    void createParser();
};

#endif // GUI_HPP
