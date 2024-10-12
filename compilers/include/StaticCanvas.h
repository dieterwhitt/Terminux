// StaticCanvas.h
// 10/12/24

#ifndef STATIC_CANVAS_H
#define STATIC_CANVAS_H

#include <string>
#include <vector>
#include "Frame.h"

using namespace std;

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
        vector<Frame> frames;
    public:
        // constructor with default values outlined in notion
        StaticCanvas(int width = 64, int height = 64, int length = 10);
        
        ~StaticCanvas();

        int get_width() const;
        int get_height() const;
        int get_length() const;

        // read function that attempts to read a .stcan file
        // returns a reference to a stack-allocated static canvas with the given data
        // must be compatible with StaticCanvas.print_stcan()
        static StaticCanvas &read_stcan(string filename);

        // prints a representation of the static canvas (.stcan) for user editing
        // metadata is clearly labeled and editable
        // frames are surrounded by bars (|) and dashes (-) for clearer editing
        void print_stcan(string filename) const;

        // compilation function with default values
        // compiles static canvas into an animatable file (.anim)
        // custom logic if filename not defined: animation{n}.anim
        void compile(int framerate = 30, bool loop = false, string filename = "") const;

        // extends the static canvas by adding blank frames
        void extend(int n, string filename);
};

#endif