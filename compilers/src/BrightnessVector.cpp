// the thing

#include "../include/BrightnessVector.h"
#include <string>
#include <cmath>
#include <cassert>

BrightnessVector::BrightnessVector(string data) : data{data} {
    assert(data.length() > 0);
}



char BrightnessVector::convert_luminance(float luminance) const {
    assert(0 <= luminance && luminance <= 255);
    // based on the length of the brightness vector l, partition into l segments.
    float l = this->data.length();
    float segment_size = 1 / l;
    // number of segments surpassed is the index to choose
    return this->data[(int) fmodf(luminance / 255, segment_size)];
}

// L = 0.2126 * R + 0.7152 * G + 0.0722 * B
char BrightnessVector::convert_rbga(int red, int blue, int green, int alpha) const {
    // use luminance * alpha (normalized)
    return convert_luminance((alpha / 255.0f) * (0.2126f * red + 0.7152f * green + 0.0722f * blue));
}

char BrightnessVector::convert_rbg(int red, int blue, int green) const {
    return convert_rbga(red, blue, green, 255);
}

char BrightnessVector::convert_hex(int hex) const {
    // bit shift to extract first 2 (red), middle 2 (green), and last 2 (blue) digits
    // use bitwise and & to mask with 0xFF, this will extract the last 2 digits

    int red = (hex >> 16) & 0xFF; // right shift 2 bytes
    int green = (hex >> 8) & 0xFF; // right shift a byte
    int blue = hex & 0xFF;
    return convert_rbg(red, green, blue);
}


