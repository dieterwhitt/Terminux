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
        // constructor 2: using given data
        Frame(vector<vector<char>> data);

        // other constructors (for module 2, 3):
        // BrightnessVector parameter + rgb/luminance matrix

        virtual ~Frame();

        int get_width() const;
        int get_height() const;
        int is_blank() const;

        // compile multiple frames into animatable (.anim)
        // custom logic if filename not defined: animation{n}.anim
        static void compile(int framerate = 30, bool loop = false, string filename = "", vector<Frame>);

        // read function that attempts to read a .frame file
        // returns a pointer to a heap-allocated frame object with the given data
        // (caller must destruct)
        // must be compatible with Frame.print_frame()
        static Frame *read_frame(string filename);

        // prints a representation of the frame (.frame)
        void print_frame(string filename) const;

        // inserts (links) the current frame into an animatable anim_filename
        // by default inserts at the end.
        void insert(string anim_filename, int posn = -1) const;

        // scales the frame to a new resolution
        // create new data vector, then approxtimate the value of each element
        // from the old data vector
        void scale(int new_width, int new_height);
};

#endif