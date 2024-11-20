// util.cpp
// 10/16/24

#include "../include/util.h"
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

// default init to white
// might not actually need this 
struct rgba {
    int r = 255;
    int g = 255;
    int b = 255;
    int a = 255;
};

string get_section(string filename, string label) {
    // read line by line and put the line in a istringstream
    // read word by word from that. once section begin is detected,
    // load all words to ostream. stop at section end. throw error if eof.
    ifstream file{filename};
    string line = "";
    string word = "";
    bool in_section = false; // in desired section
    bool done = false;
    ostringstream output{""};
    string section = "\\section{" + label + "}";
    string end = "\\end{" + label + "}";
    while (!done && getline(file, line)) {
        // first check for keywords
        istringstream line_stream{line};
        if (line_stream >> word) {
            if (word == section) {
                in_section = true;
                continue; // skip adding anything from this line
            } else if (word == end && in_section) {
                done = true;
                in_section = false;
            }
        }
        // add line when in section
        if (in_section) {
            output << line << endl;
        }
    }
    if (!done) {
        throw runtime_error{"Unable to read section " + label + "."};
    }
    file.close();
    return output.str();
}