// StaticCanvas.h
// 10/12/24

#ifndef STATIC_CANVAS_H
#define STATIC_CANVAS_H

#include <string>
#include <vector>
#include "Frame.h"

/**
 * @class StaticCanvas
 * Static Canvas for manual animation creation.
 * Metadata: x/y resolution (width and height), # of frames (length)
 * Contents: vector of frames (see frame class)
 */
class StaticCanvas {
    private:
        int width;
        int height;
        int length;
        std::vector<Frame> frames;
    public:
        // constructor with default values outlined in notion
        // creates a blank canvas (all frames are blank)
        // requires: all params > 0
        StaticCanvas(int width = 64, int height = 64, int length = 10);

        // creates a canvas from given frames (creates copy).
        // requires: frames non-empty
        StaticCanvas(std::vector<Frame> &frames);

        // future constructors: creating static canvas from 2d, 3d dynamic canvases
        // for manual tweaks to engine created animations

        int get_width() const;
        int get_height() const;
        int get_length() const;
        std::vector<Frame> get_frames() const;

        // read function that attempts to read a .stcan file
        // returns a pointer to a heap-allocated static canvas with the given data 
        // (caller must destruct)
        // must be compatible with StaticCanvas.print_stcan().
        // throws runtime exception if the file can't be read properly.
        static StaticCanvas *read_stcan(std::string filename);

        // prints a representation of the static canvas (.stcan) for user editing
        // metadata is clearly labeled and editable
        // frames are surrounded by bars (|) and dashes (-) for clearer editing
        void write_stcan(std::string filename) const;

        // compilation function with default values
        // compiles static canvas into an animatable file (.anim)
        // custom logic if filename not defined: animation{n}.anim (handled in bash)
        // requires: framerate > 0
        void compile(std::string filename, int framerate = 30, bool loop = false) const;

        // extends the static canvas by adding blank frames
        // requires: n > 0
        void extend(int n);

        // scales the canvas to a new resolution
        // create new data vector, then approxtimate the value of each element
        // from the old data vector
        // requires: int params > 0
        void scale(int new_width, int new_height);

        // resizes the canvas without any kind of scaling
        // if x/y is larger: fills in with spaces
        // if smaller: crops off
        // requires: int params > 0
        void resize(int new_width, int new_height);
};

#endif