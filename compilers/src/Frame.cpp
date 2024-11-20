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

// dependency: libpng
// brew install libpng (when we use makefile, put that shit in)
// docs from before i was born: http://www.libpng.org/pub/png/libpng-1.2.5-manual.html#section-3.9
#include <png.h>

using namespace std;

Frame::Frame(int width, int height) : width{width}, height{height}, blank{true}, data{} {
    assert(width > 0 && height > 0);
    // blank frame: fill data array with space characters
    for (int row = 0; row < height; ++row) {
        vector<char> rvec{};
        for (int col = 0; col < width; ++col) {
            rvec.push_back(' ');
        }
        this->data.push_back(rvec);
    }
}

Frame::Frame(vector<vector<char>> &data) : data{data} {
    assert(!data.empty());
    assert(!data[0].empty());
    int size = data[0].size();
    for (int row = 0; row < data.size(); ++row) {
        assert(data[row].size() == size);
    }
    this->height = data.size();
    this->width = data.at(0).size();
    this->blank = false;
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

Frame *Frame::read_png(string png_file, BrightnessVector &bv) {
    // so we're using libpng?
    // it's a c library so use c syntax
    // guide: https://jeromebelleman.gitlab.io/posts/devops/libpng/#rgb
    FILE *fp = fopen(png_file.c_str(), "rb");

    // basic error handling
    if (!fp) return nullptr;
    // read the header
    png_byte header[8];
    fread(header, 1, 8, fp);
    // test if png
    int is_png = !png_sig_cmp(header, 0, 8);
    if (!is_png) {
        fclose(fp);
        return nullptr;
    }

    // initialize png reading structures
    png_structp pngptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop pnginfo = png_create_info_struct(pngptr);
    // more error handling
    if (!pngptr || !pnginfo) {
        png_destroy_read_struct(&pngptr, &pnginfo, NULL);
        return nullptr;
    }

    // initialize stream
    png_init_io(pngptr, fp);

    // let libpng know you already read the first 8 bytes
    png_set_sig_bytes(pngptr, 8);

    // read metadata to preprocess image and prepare it for conversion 
    // (low level read interface)
    png_read_info(pngptr, pnginfo);
    // query info structure
    int height = png_get_image_height(pngptr, pnginfo);
    int width = png_get_image_width(pngptr, pnginfo); // in pixels
    int depth = png_get_bit_depth(pngptr, pnginfo);
    int color_type = png_get_color_type(pngptr, pnginfo);

    // convert color from palette/grayscale to rbg (must be done before read)
    if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(pngptr);
    else if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        png_set_gray_to_rgb(pngptr);
    }

    // hard set depth to 8 for proper reading.
    if (depth == 16)
        png_set_strip_16(pngptr);

    // cout << width << " " << height << " " << depth << endl;
    // cout << "RGB? " << (color_type == PNG_COLOR_TYPE_RGB) << endl;

    // before reading, add opaque alpha channel if it doesn't have one yet
    if (color_type == PNG_COLOR_TYPE_RGB) {
        png_set_add_alpha(pngptr, 0xff, PNG_FILLER_AFTER); // add after rbg: rgba
    }

    // last step: save input transformations before reading data
    png_read_update_info(pngptr, pnginfo);

    // begin full read
    // png_bytepp rows = new png_bytep[height];
    // allocate memory for rows: a pointer to an array of pointers
    png_bytepp rows = new png_bytep[height];

    // allocate memory for each row
    for (int i = 0; i < height; ++i) {
        rows[i] = new png_byte[width * 4];  // 4 channels: rbga
    }

    png_read_image(pngptr, rows);

    // get rows
    // png_bytepp rows = png_get_rows(pngptr, pnginfo);

    vector<vector<char>> data(height, vector<char>(width, ' '));
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width * 4; col += 4) {
            /*
            cout << "PIXEL " << row << " " << col/4 << ", RGBA: " << (int)rows[row][col] << " "
                    << (int)rows[row][col + 1] << " " << (int)rows[row][col + 2] << " "
                    << (int)rows[row][col + 3] << " "
                    << endl;
                    */
            data[row][col / 4] = bv.convert_rgba(
                    rows[row][col], 
                    rows[row][col + 1], 
                    rows[row][col + 2],
                    rows[row][col + 3]);
        }
    }
    // free stuff
    png_destroy_read_struct(&pngptr, &pnginfo, NULL);
    for (int i = 0; i < height; ++i) {
        delete[] rows[i];  // free memory for each row
    }
    delete[] rows;  // free the array of row pointers
    
    return new Frame{data};
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

void Frame::scale_factor(float factor) {
    int new_width = (int) (this->width * factor);
    int new_height = (int) (this->height * factor);
    this->scale(new_width, new_height);
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