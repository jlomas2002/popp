#ifndef WRITER_HPP
#define WRITER_HPP
#include <fstream>
#include <string>

class FileWriter {
public:
    FileWriter(std::string fileName);
    ~FileWriter();
    void writeText(std::string text, std::string mode = "");
private:
    bool firstWrite;
    int indent; //defines how many spaces to use as the current indentation level
    string indentString;
    std::string fileName;
};

#endif