// Frame.cpp
// 10/15/24

// finish frame.cpp (png read util)

#include "../include/Frame.h"

#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cassert>

using namespace std;

Frame::Frame(int width, int height) {
    assert(width > 0 && height > 0);
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
    assert(!data.empty());
    assert(!data[0].empty());
    int size = data[0].size();
    for (int row = 0; row < data.size(); ++row) {
        assert(data[row].size() == size);
    }
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
    assert(!frames.empty());
    assert(width > 0 && height > 0 && framerate > 0);

    ofstream output{filename};
    output << framerate << endl;
    output << frames.size() << endl;
    output << width << endl;
    output << height << endl;
    output << int(loop) << endl; // 1 - loop, 0 - don't loop
    for (int i = 0; i < frames.size(); ++i) {
        output << frames[i] << endl;
    }
    output.close();
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
    return result;
}

static Frame *read_frame_png(string png_file, string out_file, BrightnessVector &bv) {
    return nullptr;
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
    assert(posn >= -1);
    // read metadata
    // fps, length, width, height, loop
    array<int, 5> metadata{};
    for (int i = 0; i < 5; ++i) {
        cin >> metadata[i];
    }
    // prepare overwrite: turn ifstream into sstream
    ifstream in_stream{anim_filename};
    stringstream anim_stream{""};
    anim_stream << in_stream.rdbuf(); // copies into sstream
    in_stream.close();

    // prepare write
    ofstream out_stream{anim_filename};
    // ignore sstream metadata and write output metadata
    metadata[1] += 1;
    int trash;
    for (int i = 0; i < 5; ++i) {
        anim_stream >> trash;
        out_stream << metadata[i] << endl;
    }
    // get insert index
    if (posn == -1 || posn >= metadata[1]) {
        posn = metadata[1] - 1;
    }
    string line;
    for (int frame = 0; frame < metadata[1]; ++frame) {
        if (frame == posn) {
            out_stream << *this << endl;
            continue; // insert self for this iteration
        } else {
            // read lines
            for (int row = 0; row < metadata[3]; ++row) {
                getline(anim_stream, line);
                out_stream << line << endl;
            }
        }
    }
    out_stream.close();
}

void Frame::scale(int new_width, int new_height) {
    assert(new_width > 0 && new_height > 0);
    // to scale a dimension:
    // find scaling factor = l2/l1
    // *integer divide* the NEW tile by this factor to get the corresponding character
    // in the OLD matrix.
    vector<vector<char>> new_data(new_height, vector<char>(new_width, ' '));
    int old_row = 0;
    int old_col = 0;
    for (int row = 0; row < new_height; ++row) {
        for (int col = 0; col < new_width; ++col) {
            old_row = (row * this->height) / new_height;
            old_col = (col * this->width) / new_width;
            new_data[row][col] = this->data[old_row][old_col];
        }
    }
    this->width = new_width;
    this->height = new_height;
    this->data = new_data;
}

void Frame::resize(int new_width, int new_height) {
    assert(new_width > 0 && new_height > 0);
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
        if (row != f.get_height() - 1) {
            os << endl;
        }
    }
    return os;
}