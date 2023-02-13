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

string FileWriter::indentString(){
  string s;
  for (int i=0; i<indent; i++){
    s.append(" ");
  }
  return s;
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
    pfile<<indentString()<<"void "<<text<<"(){\n";
    indent += 4;
  }

  else if (mode == "ef"){ //end of function
    indent = 4;
    pfile<<"    }\n\n";
  }

  else if (mode == "nt"){ //non terminal
    pfile<<indentString()<<text<<"();\n";  
  }
}