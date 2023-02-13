#include <iostream>
#include "fileWriter.hpp"
using namespace std;

FileWriter::FileWriter(std::string fileName){
    ofstream file (fileName);
    pfile = &file;
    *pfile<<"This is test line"<<endl;
    *pfile<<"This is another test"<<endl; 
}

FileWriter::~FileWriter(){
  pfile->close();
}
