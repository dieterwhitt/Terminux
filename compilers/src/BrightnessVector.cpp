// the thing

#include "../include/BrightnessVector.h"
#include <string>
#include <cmath>
#include <cassert>
#include <iostream>
#include <fstream>

using namespace std;

BrightnessVector::BrightnessVector(string data) : data{data} {
    assert(data.length() > 1);
}

BrightnessVector *BrightnessVector::read_bv(string filename) {
    ifstream file{filename};
    string data;
    getline(file, data);
    file.close();
    return new BrightnessVector{data};
}

string BrightnessVector::get_data() {
    return this->data;
}

void BrightnessVector::write_bv(string filename) const {
    ofstream file{filename};
    file << this->data << endl;
    file.close();
}

char BrightnessVector::convert_luminance(float luminance) const {
    const int tolerance = 0.000001;
    assert(0 - tolerance <= luminance && luminance <= 255 + tolerance);

    // edge case: first char in vector for l = 0
    if (0 - tolerance <= luminance && luminance <= tolerance) return this->data[0];

    // based on the length of the brightness vector l, partition into l-1 segments.
    float segment_size = 1.0 / (this->data.length() - 1);
    // number of segments surpassed is the index to choose
    int index = (int) (luminance / (255 * segment_size)) + 1;
    // edge case: passed all segments (l >= 255)
    if (index == this->data.length()) --index;
    return this->data[index];
}

// L = 0.2126R + 0.7152G + 0.0722B
char BrightnessVector::convert_rgba(int red, int green, int blue, int alpha) const {
    // use luminance * alpha (normalized)
    return convert_luminance((alpha / 255.0f) * (0.2126f * red + 0.7152f * green + 0.0722f * blue));
}

char BrightnessVector::convert_rgb(int red, int green, int blue) const {
    return convert_rgba(red, green, blue, 255);
}

char BrightnessVector::convert_hex(int hex) const {
    // bit shift to extract first 2 (red), middle 2 (green), and last 2 (blue) digits
    // use bitwise and & to mask with 0xFF, this will extract the last 2 digits

    int red = (hex >> 16) & 0xFF; // right shift 2 bytes
    int green = (hex >> 8) & 0xFF; // right shift a byte
    int blue = hex & 0xFF;
    return convert_rgb(red, green, blue);
}


