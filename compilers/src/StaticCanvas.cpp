// StaticCanvas.cpp
// 10/14/24

#include "../include/StaticCanvas.h"
#include "../include/util.h"

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cassert>

using namespace std;

StaticCanvas::StaticCanvas(int width, int height, int length) : width{width}, 
        height{height}, length{length}, frames{} {
    assert(width > 0 && height > 0 && length > 0);
    // fill vector with blank frames
    for (int i = 0; i < length; ++i) {
        Frame blank{width, height};
        this->frames.push_back(blank);
    }
}

StaticCanvas::StaticCanvas(vector<Frame> &frames) : frames{frames} {
    assert(!frames.empty());
    this->width = frames[0].get_width();
    this->height = frames[0].get_height();
    this->length = frames.size();
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

const vector<Frame> &StaticCanvas::get_frames() const {
    return this->frames;
}

StaticCanvas *StaticCanvas::read_stcan(string filename) {
    // attempt to read metadata, then skip to frames section and read frames.
    // throw exception if something goes wrong: (metadata invalid/missing),
    // frames invalid/missing/wrong size. 
    string metadata = get_section(filename, "metadata");
    istringstream meta_stream{metadata};
    // metadata order: width, height, length
    array<int, 3> metadata_fields{};
    int i = 0;
    int val = 0;
    while (i < 3) {
        meta_stream >> val;
        if (meta_stream.eof()) {
            // end of metadata before numbers read: throw error
            throw runtime_error{"Invalid metadata read: " + filename};
        } else if (meta_stream.fail()) {
            // integer read fail: clear fail bit and ignore value
            meta_stream.clear();
            meta_stream.ignore();
        } else {
            // integer read
            metadata_fields[i] = val;
            ++i;
        }
    }
    // read frames
    string frames = get_section(filename, "frames");
    istringstream frame_stream{frames};
    string line = "";
    // empty 2d vector with height and width set from metadata
    vector<vector<char>> read_data{};
    vector<Frame> frame_vec{};
    for (int i = 0; i < metadata_fields[2]; ++i) {
        // clear read data for next frame
        read_data.clear();
        getline(frame_stream, line); // ignore first line (frame seperator)
        for (int row = 0; row < metadata_fields[1]; ++row) {
            getline(frame_stream, line);
            // cout << "LINE " << line << endl;
            string readable = line.substr(1, metadata_fields[0]);

            // copy readable line to vector (iterator constructor)
            vector<char> row_vec(readable.begin(), readable.end());
            read_data.push_back(row_vec);
        }
        Frame f{read_data};
        frame_vec.push_back(f);
    }
    // construct heap allocated canvas
    StaticCanvas *output = new StaticCanvas{frame_vec};
    return output;
}

// helpers for write_stcan()

int num_digits(int n) {
    if (-9 <= n && n <= 9) {
        return 1;
    } else {
        // recursive case
        return 1 + num_digits(n / 10);
    }
}

string get_seperator(int index, int width, char character, bool show_digit) {
    string output = "";
    if (show_digit) {
        output = to_string(index);
        width -= num_digits(index);
    }
    for (int i = 0; i < width; ++i) {
        output += character;
    }
    return output;
}

// sections: {metadata}, {comments}, {frames}
void StaticCanvas::write_stcan(string filename) const {
    ofstream stcan_out{filename};
    // print metadata header
    stcan_out << "Metadata: DO NOT MODIFY!" << endl;
    stcan_out << "\\section{metadata}" << endl;
    stcan_out << "Frame Width: " << this->width << endl;
    stcan_out << "Frame Height: " << this->height << endl;
    stcan_out << "Number of Frames: " << this->length << endl;
    stcan_out << "\\end{metadata}\n" << endl;

    stcan_out << "\\section{comments}\n" << endl;
    stcan_out << "Static Canvas: " << filename << "\n" << endl;
    stcan_out << "This is a Static Canvas. In this file, you can "
            "design ASCII animations with pixel-perfect accuracy!" << endl;
    stcan_out << "Create frames by editing the text segments within the rectangular "
            "borders in the frames section below.\n"
            "Ensure that the frame borders remain aligned according to the width "
            "and height of the canvas.\nWhen you're ready, "
            "compile using:\n\nterminux " << filename << "\n" << endl;
    stcan_out << "You can use this commment space to write personal notes or "
            "descriptions for your animations.\nThe sections in this file are "
            "sectioned by \\section and \\end labels.\nMake sure the lines containing "
            "these tags have no other characters.\nPlease do not modify anything "
            "within the metadata section.\nWithin the frames section, ONLY modify "
            "the content within the frame borders.\n\n\n" << endl;
    // stcan_out << "For more information on Static Canvases, visit (wiki link)!" << endl;
    stcan_out << "\\end{comments}\n" << endl;
    // print frame editing section
    stcan_out << "\\section{frames}" << endl;
    int seperator_width = this->width + 2; // +2 to account for vertical border
    char seperator_char = '-';
    string seperator = "";
    for (int f_idx = 0; f_idx < this->length; ++f_idx) {
        Frame frame = this->frames[f_idx];
        // frame to be printed: put seperator
        // number followed by dashes ('-')
        seperator = get_seperator(f_idx + 1, seperator_width, seperator_char, true);
        stcan_out << seperator << endl;
        for (int row = 0; row < this->height; ++row) {
            // put bars before and after rows
            stcan_out << '|';
            for (int col = 0; col < this->width; ++col) {
                // print row
                stcan_out << frame.get_data()[row][col];
            }
            stcan_out << '|' << endl;
        }
    }
    // close bottom of frames
    stcan_out << get_seperator(0, seperator_width, seperator_char, false) << endl;
    stcan_out << "\\end{frames}" << endl;
    stcan_out.close();
}

void StaticCanvas::compile(string filename, int framerate, bool loop) const {
    assert(framerate > 0);
    // just uses the frame class compilation (hard scaling)
    Frame::compile(filename, this->frames, this->width, this->height, false, 
            framerate, loop);
}

void StaticCanvas::extend(int n) {
    assert(n > 0);
    this->length += n;
    // create n new blank frames
    for (int i = 0; i < n; ++i) {
        Frame blank{this->width, this->height};
        this->frames.push_back(blank);
    }
}

void StaticCanvas::scale(int new_width, int new_height) {
    assert(new_width > 0 && new_height > 0);
    // scale all frames
    for (int i = 0; i < this->length; ++i) {
        this->frames[i].scale(new_width, new_height);
    }
}

void StaticCanvas::resize(int new_width, int new_height) {
    assert(new_width > 0 && new_height > 0);
    // resize all frames
    for (int i = 0; i < this->length; ++i) {
        this->frames[i].resize(new_width, new_height);
    }
}