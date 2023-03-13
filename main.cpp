#include <iostream>
#include <fstream>
#include "gui.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    /*
    if (argc != 2){
        cout<<"USAGE: Please provide a text file specifying the grammar as a command line argument" << endl; 
        return 0;
    }
    */

    //setup qt app and create the main window
    QApplication app(argc, argv);
    GUI userInterface;
    userInterface.show();

    return app.exec();
}
