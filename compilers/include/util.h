// Util.h
// 10/16/24

#ifndef UTIL_H
#define UTIL_H

#include <string>

using namespace std;

// gets a specific section in a document (whitespace included)
// from \section{label} to \end{label}. returns string
// throws runtime exception if the tags aren't found
string get_section(string filename, string label);

#endif