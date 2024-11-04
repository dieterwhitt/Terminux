// BrightnessVector.h
// 10/18/24


#ifndef BRIGHTNESS_VECTOR_H
#define BRIGHTNESS_VECTOR_H

#include <string>
#include "util.h"

using namespace std;

// small string wrapper that represents a brightness scale using characters
class BrightnessVector {
    private:
        string data;
    public:
        // constructs a brightness vector using given data
        BrightnessVector(string data);
        
        // reads a brightnessvector file .bv and returns a pointer to a heap allocated
        // brightness vector. default param = default bv filepath
        static BrightnessVector *read_bv(string filename);

        string get_data();

        // writes to a .bv file
        void write_bv(string filename);

        // all conversions below should be O(1)

        // converts a luminance (0 <= l <= 1) to a character
        char convert_luminance(float luminance);

        // converts rbga values to a character
        char convert_rbga(int red, int blue, int green, int alpha);

        // converts rbg values to a character
        char convert_rbg(int red, int blue, int green);

        // converts hex values to a character
        char convert_hex(int hex);
};

#endif
