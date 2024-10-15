// Frame.cpp
// 10/15/24

#include "../include/Frame.h"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

Frame::Frame(int width, int height) {
    this->width = width;
    this->height = height;
    this->blank = true;
    this->data = vector<vector<char>>{}; // explicitly declare empty 2d vector
    // blank frame: fill data array with space characters
    for (int row = 0; row < height; ++row) {
        vector<char> rvec{};
        for (int col = 0; col < width; ++col) {
            rvec.push_back(' ');
        }
        this->data.push_back(rvec);
    }
}

Frame::Frame(vector<vector<char>> data) {
    this->height = data.size();
    this->width = data.at(0).size();
    this->blank = false;
    this->data = data;
}

int Frame::get_width() const {
    return this->width;
}

int Frame::get_height() const {
    return this->height;
}

int Frame::is_blank() const {
    return this->blank;
}

const vector<vector<char>> &Frame::get_data() const {
    return this->data;
}

void Frame::compile(string filename, const vector<Frame> &frames, int width, int height, 
        bool scale, int framerate = 30, bool loop = false) {

    }

Frame *Frame::read_frame(string filename) {

    return nullptr;
}

void Frame::write_frame(string filename) const {
    ofstream frame_out{filename};
    // write width, height, then frame data, end with newline
    frame_out << this->width << endl;
    frame_out << this->height << endl;
    frame_out << *this << endl;
}

void Frame::insert(string anim_filename, int posn = -1) const {

}

void Frame::scale(int new_width, int new_height) {

}

void Frame::resize(int new_width, int new_height) {

}

ostream &operator<<(ostream &os, const Frame &f) {
    for (int row = 0; row < f.get_height(); ++row){
        for (int col = 0; col < f.get_width(); ++col) {
            os << f.get_data()[row][col];
        }
        // add newline after all rows except last
        if (row == f.get_height() - 1) {
            os << endl;
        }
    }
}