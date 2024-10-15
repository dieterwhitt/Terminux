// StaticCanvas.cpp
// 10/14/24

#include "../include/StaticCanvas.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

StaticCanvas::StaticCanvas(int width = 64, int height = 64, int length = 10, 
        vector<Frame> frames = {}) {
    this->width = width;
    this->height = height;
    this->length = length;
    if (frames.empty()) {
        // fill vector with blank frames
        for (int i = 0; i < frames.size(); ++i) {
            Frame blank{width, height};
            frames.push_back(blank);
        }
        this->frames = frames;
    } else {
        this->frames = frames;
    }
}

int StaticCanvas::get_width() const {
    return this->width;
}

int StaticCanvas::get_height() const {
    return this->height;
}

int StaticCanvas::get_length() const {
    return this->length;
}

StaticCanvas *StaticCanvas::read_stcan(string filename) {

}

void StaticCanvas::print_stcan(string filename) const {

}

void StaticCanvas::compile(int framerate = 30, bool loop = false, string filename = "") const {

}

void StaticCanvas::extend(int n, string filename) {

}