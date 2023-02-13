#include <iostream>
#include "fileWriter.hpp"
using namespace std;

FileWriter::FileWriter(std::string name){
    if (name == ""){
      ;
    }
    else{
      indent = 4;
      firstWrite = true;
      fileName = name;
    }
}

FileWriter::~FileWriter(){
  ;
}

void FileWriter::writeText(string text, string mode){
  ofstream pfile;
  if (firstWrite){
    pfile.open(fileName, std::ios_base::out);
    firstWrite = false;
  }
  else{
    pfile.open(fileName, std::ios_base::app); //append to file
  }

  if (mode == ""){
    if (pfile.is_open()){
        pfile<<text<<"\n";
    }
    else{
      cout << "File could not be opened"<<endl;
    }
    if (pfile.fail() || pfile.bad()){
      cout<<"File has failed"<<endl;
    }
  }

  else if (mode == "fh"){ //function header
    pfile<<"void "<<text<<"(){\n";
  }

  else if (mode == "addBrackets"){

  }

  else if (mode == "nt"){ //non terminal
    pfile<<text<<"();\n";  
  }
}