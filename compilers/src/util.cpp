// util.cpp
// 10/16/24

#include "../include/util.h"
#include "../include/Frame.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <cassert>

#include <opencv2/opencv.hpp>

using namespace std;

// default init to white
// might not actually need this 
struct rgba {
    int r = 255;
    int g = 255;
    int b = 255;
    int a = 255;
};

// todo refactor to take string input, not filename input. (caller will open file)

string get_section(string filename, string label) {
    // read line by line and put the line in a istringstream
    // read word by word from that. once section begin is detected,
    // load all words to ostream. stop at section end. throw error if eof.
    ifstream file{filename};
    string line = "";
    string word = "";
    bool in_section = false; // in desired section
    bool done = false;
    ostringstream output{""};
    string section = "\\section{" + label + "}";
    string end = "\\end{" + label + "}";
    while (!done && getline(file, line)) {
        // first check for keywords
        istringstream line_stream{line};
        if (line_stream >> word) {
            if (word == section) {
                in_section = true;
                continue; // skip adding anything from this line
            } else if (word == end && in_section) {
                done = true;
                in_section = false;
            }
        }
        // add line when in section
        if (in_section) {
            output << line << endl;
        }
    }
    if (!done) {
        throw runtime_error{"Unable to read section " + label + "."};
    }
    file.close();
    return output.str();
}

std::pair<int, int> determine_scaling(int target_x, int target_y, float s, 
        int x, int y) {
    assert(s >= 0 && x >= 0 && y >= 0);
    // s takes precedent: check first
    if (s > 0) {
        int out_x = (int) (target_x * s);
        int out_y = (int) (target_y * s);
        if (out_x == 0) ++out_x;
        if (out_y == 0) ++out_y;
        return pair{out_x, out_y};
    } else if (x > 0 && y > 0) {
        // x and y both defined.
        return pair{x, y};
    } else if (x > 0) {
        // only x defined
        float ratio = ((float) (x)) / ((float) (target_x));
        int out_y = (int) (ratio * target_y);
        return pair{x, out_y};
    } else if (y > 0) {
        // only y defined
        float ratio = ((float) (y)) / ((float) (target_y));
        int out_x = (int) (ratio * target_x);
        return pair{out_x, y};
    } else {
        // neither defined. don't scale
        return pair{target_x, target_y};
    }
}

// if params are not provided, default values will be handled accordingly
// currently only reads colour videos. (rgb). cv::Mat::type() = CV_8UC3 (8 bit unsigned, 3 channels)
void compile_video(string vid_filename, string out_filename, BrightnessVector &bv, 
        float scale_factor, int scaled_x, int scaled_y, bool resize, int fps,
        bool loop) {
    // create vid capture object
    cv::VideoCapture capture{vid_filename};
    if (!capture.isOpened()) {
        throw runtime_error{"unable to open video: " + vid_filename};
    }
    // get relevant metadata of video
    int width = (int) (capture.get(cv::CAP_PROP_FRAME_WIDTH));
    int height = (int) (capture.get(cv::CAP_PROP_FRAME_HEIGHT));
    if (fps == 0) fps = (int) (capture.get(cv::CAP_PROP_FPS));

    cout << "VIDEO FOUND. WIDTH: " << width << " HEIGHT: " << height << " FPS: " << fps << endl;

    // dimensions of anim
    std::pair dimensions = determine_scaling(width, height, scale_factor, scaled_x, scaled_y);

    vector<Frame> frames;
    cv::Mat mat_frame; // a single frame

    int count = 0;
    while (capture.read(mat_frame)) {
        vector<vector<char>> data(height, vector<char>(width, ' '));

        for (int row = 0; row < height; ++row) {
            for (int col = 0; col < width; ++col) {
                // get the RGB values of the pixel
                cv::Vec3b pixel = mat_frame.at<cv::Vec3b>(row, col);
                 // BGR format in opencv
                unsigned char blue = pixel[0];
                unsigned char green = pixel[1];
                unsigned char red = pixel[2];
                char pixel_char = bv.convert_rgb(red, green, blue);
                data[row][col] = pixel_char;
            }
        }
        // create frame object and scale/resize it
        Frame frame_obj{data};
        
        if (resize) {
            frame_obj.resize(dimensions.first, dimensions.second);
        } else {
            frame_obj.scale(dimensions.first, dimensions.second);
        }
        frames.push_back(frame_obj);
    }
    // compile
    Frame::compile(out_filename, frames, dimensions.first, 
            dimensions.second, false, fps, loop);
}
