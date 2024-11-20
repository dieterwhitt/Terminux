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

#endif