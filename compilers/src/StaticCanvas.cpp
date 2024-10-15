// StaticCanvas.cpp
// 10/14/24

#include "../include/StaticCanvas.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>

using namespace std;

StaticCanvas::StaticCanvas(int width, int height, int length) {
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

vector<Frame> StaticCanvas::get_frames() const {
    return this->frames;
}

StaticCanvas *StaticCanvas::read_stcan(string filename) {
    // attempt to read metadata, then skip to frames section and read frames.
    // throw exception if something goes wrong: (metadata invalid/missing),
    // frames invalid/missing/wrong size

    return nullptr;
}

void StaticCanvas::write_stcan(string filename) const {
    ofstream stcan_out{filename};
    string frame_seperator(this->width, '-');
    // print metadata header
    stcan_out << "Metadata: DO NOT MODIFY!" << endl;
    stcan_out << "\section{metadata}" << endl;
    stcan_out << "Width: " << this->width << endl;
    stcan_out << "Height: " << this->height << endl;
    stcan_out << "Length: " << this->length << endl;
    stcan_out << "\end{metadata}" << endl;

    stcan_out << "\section{comments}" << endl;
    stcan_out << "Static Canvas: " << filename << endl;
    stcan_out << "This is a Static Canvas. In this file, you can "
            "design ASCII animations with pixel-perfect accuracy!" << endl;
    stcan_out << "Create frames by editing the text segments within the rectangular "
            "borders in the frames section below. "
            "Ensure that the frame borders remain aligned according to the width "
            "and height of the canvas shown in the metadata above. When you're ready, "
            "compile using" << endl << "terminux " << filename << endl;
    stcan_out << "You can use this commment space to write personal notes or "
            "descriptions for your animations. The sections in this file are "
            "sectioned by \section and \end labels. Please do not modify anything "
            "within the metadata section. Within the frames section, ONLY modify"
            "the content within the frame borders" << endl << endl << endl << endl;
    // stcan_out << "For more information on Static Canvases, visit (wiki link)!" << endl;
    stcan_out << "\end{comments}" << endl;
    // print frame editing section
    stcan_out << "\section{frames}" << endl;
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
    stcan_out << "\end{frames}" << endl;
    stcan_out.close();
}

void StaticCanvas::compile(string filename, int framerate, bool loop) const {
    // just uses the frame class compilation (hard scaling)
    Frame::compile(filename, this->frames, this->width, this->height, false, 
            framerate, loop);
}

void StaticCanvas::extend(int n) {
    this->length += n;
    // create n new blank frames
    for (int i = 0; i < n; ++i) {
        Frame blank{this->width, this->height};
        this->frames.push_back(blank);
    }
}

void StaticCanvas::scale(int new_width, int new_height) {
    // scale all frames
    for (int i = 0; i < this->length; ++i) {
        this->frames[i].scale(new_width, new_height);
    }
}

void StaticCanvas::resize(int new_width, int new_height) {
    // resize all frames
    for (int i = 0; i < this->length; ++i) {
        this->frames[i].resize(new_width, new_height);
    }
}