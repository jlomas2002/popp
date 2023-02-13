#ifndef WRITER_HPP
#define WRITER_HPP
#include <fstream>
#include <string>

class FileWriter {
public:
    FileWriter(std::string fileName);
    ~FileWriter();
private:
    std::ofstream *pfile;

};

#endif