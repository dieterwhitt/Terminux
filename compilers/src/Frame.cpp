// Frame.cpp
// 10/15/24

#include "../include/Frame.h"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <stdexcept>

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
        bool scale, int framerate, bool loop) {

    }

Frame *Frame::read_frame(string filename) {
    ifstream frame_in{filename};
    // read metadata (width, height)
    int r_width;
    int r_height;
    frame_in >> r_width >> r_height;
    if (frame_in.fail()) {
        throw runtime_error("Unable to read " + filename + " metadata.");
    }
    // fill data vector for frame construction
    vector<vector<char>> r_data(r_height, vector<char>(r_width, ' '));
    char pixel;
    for (int row = 0; row < r_height; ++row) {
        for (int col = 0; col < r_width; ++col) {
            frame_in >> pixel;
            if (frame_in.eof()) {
                throw runtime_error("Unable to read " + filename + " frame data.");
            }
            r_data[row][col] = pixel;
        }
    }
    // create heap allocated frame from data
    Frame *result = new Frame{r_data};
    frame_in.close();
}

void Frame::write_frame(string filename) const {
    ofstream frame_out{filename};
    // write width, height, then frame data, end with newline
    frame_out << this->width << endl;
    frame_out << this->height << endl;
    frame_out << *this << endl;
    frame_out.close();
}

void Frame::insert(string anim_filename, int posn) const {

}

void Frame::scale(int new_width, int new_height) {

}

void Frame::resize(int new_width, int new_height) {
    // initialize new sized vector filled with spaces
    vector<vector<char>> new_data(new_height, vector<char>(new_width, ' '));
    for (int row = 0; row < this->height; ++row) {
        // check that row is still valid in new matrix
        if (row >= new_height) {
            break;
        }
        for (int col = 0; col < this->width; ++col) {
            // check that col is still valid in new matrix
            if (col >= new_width) {
                break;
            }
            // set value in new matrix
            new_data[row][col] = this->data[row][col];
        }
    }
    this->data = new_data;
    this->width = new_width;
    this->height = new_height;
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
    return os;
}