// Util.h
// 10/16/24

#ifndef UTIL_H
#define UTIL_H

#include <string>
#include "BrightnessVector.h"

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
// if s is provided, scale by a factor. if only x or y are provided, set and scale accordingly.
// if none are provided, just use the target's existing resolution
// outputs a pair {x, y}
std::pair<int, int> determine_scaling(int target_x, int target_y, float s = 0.0f, 
        int x = 0, int y = 0);

// see notion docs: terminux compile [.mp4]
// WARNING: the brightness vector must be created before hand.
// since the intended command accepts an optional filename of a brightness vector,
// the bv must be created by a function wrapping this one (routed by command router)
// hopefully that makes sense
void compile_video(std::string vid_filename, std::string out_filename, 
        BrightnessVector &bv, float scale_factor = 0.0f, 
        int scaled_x = 0, int scaled_y = 0, bool resize = false, int fps = 0,
        bool loop = false);

#endif