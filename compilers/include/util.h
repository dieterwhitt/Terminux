// Util.h
// 10/16/24

#ifndef UTIL_H
#define UTIL_H

#include <string>

// utilities (structs, helper functions, etc.)

// a single rgba pixel (red, green, blue, alpha) /255
struct rgba;

// gets a specific section in a document (whitespace included)
// from \section{label} to \end{label}. returns string
// throws runtime exception if the tags aren't found
std::string get_section(std::string filename, std::string label);

// removes comments from text.
// a line beginning with '#' is a comment.
std::string strip_comments(std::string content);

// removes whitespace lines from text.
// a whitespace line is a line containing only whitespace.
std::string strip_white_lines(std::string content);

// finds the scale resolution when parsing -x, -y, -s flags based on a 
// target's resolution, according to the behaviour outlined in notion:
// s - scale factor
// x - scaled x
// y - scaled y
// outputs a pair {x, y}
std::pair<int, int> determine_scaling(int target_x, int target_y, float s = 0.0f, 
        int x = 0, int y = 0);

#endif