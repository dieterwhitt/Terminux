// StaticCanvas.cpp
// 10/14/24

#include "../include/StaticCanvas.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

StaticCanvas::StaticCanvas(int width = 64, int height = 64, int length = 10) {
    this->width = width;
    this->height = height;
    this->length = length;
    this->frames = {};
    // fill vector with blank frames
    for (int i = 0; i < frames.size(); ++i) {
        Frame blank{width, height};
        this->frames.push_back(blank);
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

vector<Frame> get_frames() const {
    return this->frames;
}

StaticCanvas *StaticCanvas::read_stcan(string filename) {

}

void StaticCanvas::write_stcan(string filename) const {
    ofstream stcan_out{filename};
    // file seperators
    string meta_seperator(this->width, '*');
    string frame_seperator(this->width, '-');
    // print metadata header
    stcan_out << meta_seperator << endl;
    stcan_out << "Metadata: DO NOT MODIFY!" << endl;
    stcan_out << "Width: " << this->width << endl;
    stcan_out << "Height: " << this->height << endl;
    stcan_out << "Length: " << this->length << endl;
    stcan_out << meta_seperator << endl << endl;

    stcan_out << "Static Canvas: " << filename << endl;
    stcan_out << "This is a Static Canvas. In this file, you can precisely "
            "design your ASCII animations with pixel-perfect accuracy!" << endl;
    stcan_out << "Create frames by editing the text segments within the frame "
            "borders in the section below. "
            "Ensure that the frame borders remain aligned according to the width "
            "and height of the canvas shown in the metadata above. When you're ready, "
            "compile using" << endl << "terminux " << filename << endl;
    // stcan_out << "For more information on Static Canvases, visit (wiki link)!" << endl;
    stcan_out << endl << endl << "FRAMES BEGIN: EDIT BELOW" << endl << endl;
    // print frame editing section
    stcan_out << frame_seperator << endl;
    for (int f_idx = 0; f_idx < this->frames.size(); ++f_idx) {
        Frame frame = this->frames[f_idx];
        for (int row = 0; row < this->height; ++row) {
            // put bars before and after rows
            stcan_out << '|';
            for (int col = 0; col < this->width; ++col) {
                // print row
                stcan_out << frame.get_data()[row][col];
            }
            stcan_out << '|' << endl;
        }
        // frame printed: put seperator
        stcan_out << frame_seperator << endl;
    }
    stcan_out.close();
}

void StaticCanvas::compile(int framerate = 30, bool loop = false, string filename = "") const {

}

void StaticCanvas::extend(int n, string filename) {

}

void scale(int new_width, int new_height) {

}

void resize(int new_width, int new_height) {

}