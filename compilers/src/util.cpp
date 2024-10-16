// util.cpp
// 10/16/24

#include "../include/util.h"
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

string get_section(string filename, string label) {
    // read line by line and put the line in a istringstream
    // read word by word from that. once section begin is detected,
    // load all words to ostream. stop at section end. throw error if eof.
    ifstream file{filename};
    string line = "";
    string word = "";
    bool in_section = false;
    bool done = false; // done reading section
    ostringstream output{""};
    string section = "\\section{" + label + "}";
    string end = "\\end{" + label + "}";
    bool first_word = true; // for spaces between words
    // extra logic: if \begin is the only word in the line: skip the newline right after
    bool add_newline = false;
    while (!done && getline(file, line)) {
        cout << "LINE: " << line << endl;
        istringstream line_stream{line};
        first_word = true;
        while (!done && line_stream >> word) {
            cout << "WORD: " << word << endl;
            if (!in_section && word == section) {
                add_newline = false; // do not add newline directly after \section
                in_section = true;
            } else if (in_section && word == end) {
                done = true;
                add_newline = false;
            } else if (in_section) {
                // reading section word
                if (!first_word) {
                    output << " ";
                }
                first_word = false;
                add_newline = true;
                output << word;
            }
        }
        // end of line: add newline
        if (add_newline) {
            cout << "ADDING NEWLINE" << endl;
            output << endl;
        }
        // after skipping a newline in a section, add all newlines after
        if (in_section) {
            add_newline = true;
        }
    }
    if (!done) {
        throw runtime_error{"Unable to read section " + label + "."};
    }
    file.close();
    return output.str();
}