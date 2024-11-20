// the thing

#include "../include/BrightnessVector.h"
#include <string>
#include <cmath>
#include <cassert>
#include <iostream>

BrightnessVector::BrightnessVector(string data) : data{data} {
    assert(data.length() > 0);
}

char BrightnessVector::convert_luminance(float luminance) const {
    const int tolerance = 0.000001;
    assert(0 - tolerance <= luminance && luminance <= 255 + tolerance);
    // floating point precision errors: cap in range 0, 255
    if (luminance < 0) luminance = 0.0f;
    else if (luminance >= 255) luminance = 255.0f;
    // based on the length of the brightness vector l, partition into l segments.
    float l = this->data.length();
    float segment_size = 1 / l;
    // number of segments surpassed is the index to choose
    int index = (int) (luminance / (255 * segment_size));
    // edge case: passed all segments (l = 255)
    if (index == l) --index;
    return this->data[index];
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


