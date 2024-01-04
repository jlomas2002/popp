## Popp
Popp (Popp Originates Predictive Parsers) is a parser generator that generates human-readable recursive descent parsers from EBNF grammars.
The app can be run via a GUI or the command line. A parser and a lexer is generated from an EBNF grammar and an optional 'tokens' file which
specifies the regexes of any lexical tokens. For example, a digit token would be specified as:

digit = \\d;

The app offers lexers and parser in 3 languages: c++, java and python.
The generated lexer will skip any whitespace and return the next character in the input unless the input is matched to a regex from a tokens file.
The generated parser includes a main function that expects a file containing a string to parse as a command line argument. The string is then passed to the lexer and the parser calls functions from the lexer to get or peek tokens during parsing.
If the string is syntactically incorrect according to the grammar, the parser prints a precise error message and terminates.
If parsing is successful, the parser prints a success message and terminates.

The GUI also has a test string feature where a user can input a test string, and the generated parser and lexer will be used to output if the string is syntactically correct.

## Build Instructions 
To build the application you will need the following prerequesites installed: Qt and a compatible C++ compiler (I used MinGW's g++ for windows, but Qt has others available).

First, pull this repositry.\
The popp.pro file is used to build the application - you can either build via the command line or with Qt Creator.\
The original development was done on a Windows 11 machine, but using the .pro file should work on any Qt compatible system.\

**Building with Qt creator**\
Double clicking popp.pro should open up the project in Qt creator.\ 
From here, assuming you have a compatible C++ compiler installed, you should be able to select a Qt kit to use to build the application.\
Once this is set up you should be able to press the green arrow in the bottom left to build and run the application.\ 
You can select whether to build and run in release or debug mode. 

**Building via the command line**\
Open a terminal at the same level as popp.pro.\
Run the command **qmake**\
Run the command **make**\
(These commands may need adding to the Path environment variable).\
The following link may help: [Building via the command line](https://wiki.qt.io/Getting_Started_on_the_Commandline).\
To run the application, locate the executable popp.exe and double click or run via the command line.

The executable and build files may be created outside of the main project folder depending on if the Shadow Build option is selected in Qt Creator. 
If you wish to have the executable and build files be created inside of the project folder, you can uncheck this box by clicking on Projects in the side panel and selecting build under the Build and Run section. Here you can find the Shadow Build setting and turn it off/on in release or debug mode.

## Usage
Double clicking the executable will open up the GUI. Alternatively, you can run via the command line.

If you pass 4 command line arguments of the following form:

projectName targetLanguage pathToGrammarFile pathToTokenDefinitionFile

Then a parser and lexer named after projectName in written in targetLanguage will be generated.
Note, targetLanguage can be 'python' 'cpp' or 'java'
If you do not have a token type definition file, replace the path with 'None'.

If you run the exectuable in the command line with no arguments, it opens up the GUI.

## License
License to be decided.
