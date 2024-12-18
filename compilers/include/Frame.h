// Frame.h
// 10/12/24

// (unfinished)

#ifndef FRAME_H
#define FRAME_H

#include <string>
#include <vector>
#include "BrightnessVector.h"

/**
 * @class Frame
 * Frame representation. Contains metadata (blank, width, height) and 2d vector of chars.
 */
class Frame {
    private:
        int width;
        int height;
        bool blank;
        std::vector<std::vector<char>> data;
    public:
        // constructor 1 (default constructor): creates blank frame
        // requires: width, height > 0
        Frame(int width = 64, int height = 64);
        // constructor 2: using given data (creates copy)
        // requires: data vector is non-empty and all rows are equal length
        Frame(std::vector<std::vector<char>> &data);

        // other constructors (for module 2, 3):
        // BrightnessVector parameter + rgb/luminance matrix

        int get_width() const;
        int get_height() const;
        int is_blank() const;

        // returns a const reference (read-only) of data
        const std::vector<std::vector<char>> &get_data() const;

        // todo: adjust compile in accordance with behavior outline in notion

        // compile multiple frames into animatable (.anim)
        // custom logic if filename not defined: animation{n}.anim (handled in bash)
        // width and height must be defined. if scale is True, it will soft scale,
        // otherwise it will hard scale (cutoff)
        // requires: frames non empty, all int params > 0
        static void compile(std::string filename, const std::vector<Frame> &frames, int width, int height, 
                bool scale, int framerate = 30, bool loop = false);

        // read function that attempts to read a .frame file
        // returns a pointer to a heap-allocated frame object with the given data
        // (caller must free)
        // must be compatible with Frame.print_frame().
        // throws runtime exception if invalid.
        static Frame *read_frame(std::string filename);

        // read function that attempts to read a .png
        // returns a pointer to a heap-allocated frame object with the given data
        // (caller must free)
        // throws runtime exception if invalid.
        // needs brightness vector class too.
        static Frame *read_png(std::string png_file, BrightnessVector &bv);

        // writes a representation of the frame (.frame)
        void write_frame(std::string filename) const;

        // inserts (links) the current frame into an animatable anim_filename
        // by default inserts at the end.
        // does not scale proportionally to fit: images inserted using resize()
        // assumes animatable is valid! OVERWRITES the original animatable.
        // requires: -1 <= posn 
        void insert(std::string anim_filename, int posn = -1) const;

        // scales the frame proportionally to a new resolution (soft scale)
        // create new data vector, then approxtimate the value of each element
        // from the old data vector
        // requires: int params > 0
        void scale(int new_width, int new_height);

        // scales by a factor while maintining the aspect ratio of the frame
        void scale_factor(float factor);

        // resizes the frame without any kind of scaling (hard scale)
        // if x/y is larger: fills in with spaces
        // if smaller: crops off
        // requires: int params > 0
        void resize(int new_width, int new_height);
};

// overload printing to print frames (only data matrix is printed)
// does NOT end with a newline.
std::ostream &operator<<(std::ostream &os, const Frame &f);

#endif