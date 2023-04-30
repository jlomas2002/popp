#ifndef TOKENISER_HPP
#define TOKENISER_HPP

#include <string>
#include <vector>

#include "tokenDef.hpp"

bool operator<(const Gtoken &a, const Gtoken &b);
bool operator==(const Gtoken &a, const Gtoken &b);

extern std::set<char> reservedSymbols;

/**
 * @brief Tokeniser - object handles all lexical analyses of grammars and token inputs.
 *
 * Also collects information about grammar such as the first set of each terminal.
 */
class Tokeniser {
public:
    /**
        * @brief Constructor which creates a new Tokeniser.
        * 
        * 
        * Performs a first pass over the grammar to collect the first set of each non terminal and detect errors.
        * Extracts any regexes from the tokens string and stores them.
        *
        * @param grammar A string storing the grammar to tokenise.
        * @param tokens A string storing any defimitions of tokens.
        * @return A new tokeniser.
    */
    Tokeniser(std::string grammar, std::string tokens);
    /**
     * Tokeniser destructor.
    */
    ~Tokeniser();
    /**
     * @brief Returns the next token in the input doesn't consume it. 
     * 
     * @return Gtoken 
     */
    Gtoken peekNextToken();
    /**
     * @brief Returns the next token and consumes it from the input string.
     * 
     * @return Gtoken 
     */
    Gtoken getNextToken();
    /**
     * @brief Creates a token assigned to an error type and returns it.
     * 
     * @param err The error enum that was detected.
     * @param lex The lexeme generated when the error was found.
     * @return Gtoken that represents the error found. 
     */
    Gtoken makeErrorToken(Error err, std::string lex);
    /**
     * @brief Returns the vector that stores the first set for each nonterminal.
     * 
     * @return std::vector<FirstSetInfo> 
     */
    std::vector<FirstSetInfo> getAllFirstSetInfo();
    /**
     * @brief Returns the set of all lexical token ids from the tokens input.
     * 
     * @return std::set<std::string> 
     */
    std::set<std::string> getListOfTokens();
    /**
     * @brief Returns the vector of structs which store a token id and its regex.
     * 
     * @return std::vector<TokenRegex> 
     */
    std::vector<TokenRegex> getTokenRegexes();
    /**
     * @brief Returns whether an error was detected in the grammar or tokens by the tokeniser.
     * 
     * @return true 
     * @return false 
     */
    bool getErrorState();
    /**
     * @brief Returns the token representing an error in the tokens input.
     * 
     * @return Gtoken 
     */
    Gtoken getTokenFileError();
    /**
     * @brief Returns the token representing an error in the grammar input.
     * 
     * @return Gtoken 
     */
    Gtoken getGrammarFileError();
    /**
     * @brief Utility function to convert an error enum to a string.
     * 
     * @param err The error enum.
     * @return The enum in string form.
     */
    std::string errorToString(Error err);
private:
    /**
     * @brief Collects the first set of each non terminal and a set of all non terminals.
     * 
     */
    void firstPass();
    /**
     * @brief Loops through the grammar and collects terminals/tokens that can begin a non terminal.
     * 
     */
    void collectFirstSetInfo();
    /**
     * @brief Loops through the first set info and adds each non terminal to a set.
     * 
     * @return Returns error token if there is a redefined element.
     */
    Gtoken collectListOfNonTerminals();
    /**
     * @brief Loops through each non terminals first set and converts any non terminals into their first set
     * 
     * @param returnInfo The firstSet to add terminals/tokens to.
     * @param currentInfo The current first set.
     */
    void refineFirstSetInfo(FirstSetInfo &returnInfo, FirstSetInfo &currentInfo);
    /**
     * @brief Returns the first set information matching the non terminal passed in.
     * 
     * @param t The token representing the non terminal. 
     * @return FirstSetInfo The first set info corresponding to the non terminal.
     */
    FirstSetInfo findFirstSetInfo(Gtoken t);
    /**
     * @brief Adds 1 to the index of the grammar file or tokens input.
     * 
     * @param input Either the grammar or tokens
     * @return 0 if increment successfully. 1 if the index exceeds input size. 
     */
    int incrementIndex(std::string input);
    /**
     * @brief Checks if a token has been defined in a token file.
     * 
     * @param id The name of the token. 
     * @return Returns an undefinedToken enum if a token file exists but this token wasn't defined in it. 
     */
    Error checkUndefinedError(std::string id);
    /**
     * @brief Goes through the tokens input and stores each tokens name and regex.
     * 
     * @param input The tokens input. 
     * @return Returns error token if their is syntactic error in tokens file. 
     */
    Gtoken extractRegexes(std::string input);

    //Stores the first set of each non terminal in a grammar.
    std::vector<FirstSetInfo> allFirstSetInfo;
    //Stores a set of all the non terminals in a grammar.
    std::set<std::string> listOfNonTerminals;
    //Stores a set of all the token ids in a token file.
    std::set<std::string> listOfTokens;
    //Stores a vector of a tokens id and its regex.
    std::vector<TokenRegex> tokenRegexes;

    //Stores the grammar input
    std::string grammar;

    //Stores the current position in the current rule of the grammar.
    int pos;
    //Stores the current rule number of the grammr.
    int lineNum;
    //Stores the current index when processing grammar.
    int index;
    //Bool storing if a user has defined tokens.
    bool tokenFileExists;
    //Stores whether the grammar has already been processed once.
    bool secondPass;

    //Stores whether an error was detected in grammar or tokens input.
    bool erroneous;
    //Token representing an error in tokens input.
    Gtoken tokenFileError;
    //Token representing an error in grammar input.
    Gtoken grammarFileError;
    
};

#endif
