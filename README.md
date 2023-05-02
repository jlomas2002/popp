# Popp

## Popp
Popp stands for Popp Originates Predictive Parsers. The name illustrates the function of the app, generating predictive parsers
(specifically Recursive Descent), and plays on the Recursive Descent output by being a recursive acronym.
The app can be run via a GUI or the command line. A parser and a lexer is generated from a EBNF grammar, and an optional 'tokens' file which
specifies the regexes of any lexical tokens. For example, a digit token would be specified as:

digit = \\d;

See the Usage section for a detailed explantion on defining a grammar and tokens.

The app offers lexers and parser in 3 languages: c++, java and python.
The generated lexer will skip any whitespace and return the next character in the input unless the input is matched to a regex from a tokens file.
The generated parser includes a main function that expects a file containing a string to parse as a command line argument. The string is then passed to the lexer and the parser calls functions from the lexer to get or peek tokens during parsing.
If the string is syntactically incorrect according to the grammar, the parser prints a precise error message and terminates.
If parsing is successful, the parser prints a success message and terminates.

The GUI also has a test string feature where a user can input a test string, and the generated parser and lexer will be used to output if the string is syntactically correct.

## Installation
Pull the repositry.
Open the application using the .pro file.
Build the project using Qt and run

Note some setting s in files may need changing to run the project on different machines and operating systems.
Also, the path to  icons used in the GUI need to be changed  in GUI.cpp to reflect the correct path the pngs are stored in.

## Usage
Run the Qt application. No commands supplied will open the GUI. If you pass comand line arguments of the following form:

projectName targetLanguage pathToGrammarFile pathToTokenDefinitionFile

then a parser and lexer named after projectName in written in targetLanguage will be generated.
Note, targetLanguage can be 'python' 'cpp' or 'java'
If you do not have a token type definition file, replace the path with 'None'.

## License
License to be decided.
