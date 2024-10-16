// Frame.h
// 10/12/24

// (unfinished)

#ifndef FRAME_H
#define FRAME_H

#include <string>
#include <vector>

using namespace std;

/**
 * @class Frame
 * Frame representation. Contains metadata (blank, width, height) and 2d vector of chars.
 */
class Frame {
    private:
        int width;
        int height;
        bool blank;
        vector<vector<char>> data;
    public:
        // constructor 1: creates blank frame
        Frame(int width, int height);
        // constructor 2: using given data (creates copy)
        Frame(vector<vector<char>> data);

        // other constructors (for module 2, 3):
        // BrightnessVector parameter + rgb/luminance matrix

        int get_width() const;
        int get_height() const;
        int is_blank() const;
        // returns a const reference (read-only) of data
        const vector<vector<char>> &get_data() const;

        // compile multiple frames into animatable (.anim)
        // custom logic if filename not defined: animation{n}.anim (handled in bash)
        // width and height must be defined. if scale is True, it will soft scale,
        // otherwise it will hard scale (cutoff)
        static void compile(string filename, const vector<Frame> &frames, int width, int height, 
                bool scale, int framerate = 30, bool loop = false);

        // read function that attempts to read a .frame file
        // returns a pointer to a heap-allocated frame object with the given data
        // (caller must destruct)
        // must be compatible with Frame.print_frame().
        // throws runtime exception if invalid.
        static Frame *read_frame(string filename);

        // read function that attempts to read a .png
        // returns a pointer to a heap-allocated frame object with the given data
        // throws runtime exception if invalid.
        static Frame *read_frame_png(string png_file, string out_file);

        // writes a representation of the frame (.frame)
        void write_frame(string filename) const;

        // inserts (links) the current frame into an animatable anim_filename
        // by default inserts at the end.
        void insert(string anim_filename, int posn = -1) const;

        // scales the frame proportionally to a new resolution (soft scale)
        // create new data vector, then approxtimate the value of each element
        // from the old data vector
        void scale(int new_width, int new_height);

        // resizes the frame without any kind of scaling (hard scale)
        // if x/y is larger: fills in with spaces
        // if smaller: crops off
        void resize(int new_width, int new_height);
};

// overload printing to print frames (only data matrix is printed)
// does NOT end with a newline.
ostream &operator<<(ostream &os, const Frame &f);

#endif