// util.cpp
// 10/16/24

#include "../include/util.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <cassert>

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

std::pair<int, int> determine_scaling(int target_x, int target_y, float s = 0.0f, 
        int x = 0, int y = 0) {
    assert(s >= 0 && x >= 0 && y >= 0);
    // s takes precedent: check first
    if (s > 0) {
        int out_x = (int) (target_x * s);
        int out_y = (int) (target_y * s);
        if (out_x == 0) ++out_x;
        if (out_y == 0) ++out_y;
        return pair{out_x, out_y};
    } else if (x > 0 && y > 0) {
        // x and y both defined.
        return pair{x, y};
    } else if (x > 0) {
        // only x defined
        float ratio = ((float) (x)) / ((float) (target_x));
        int out_y = (int) (ratio * target_y);
        return pair{x, out_y};
    } else if (y > 0) {
        // only y defined
        float ratio = ((float) (y)) / ((float) (target_y));
        int out_x = (int) (ratio * target_x);
        return pair{out_x, y};
    } else {
        // neither defined. don't scale
        return pair{target_x, target_y};
    }
}