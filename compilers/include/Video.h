// Video.h
// 10/14/24

#ifndef VIDEO_H
#define VIDEO_H

#include <string>
#include <vector>

// class specifically for compiling videos (.mp4) into animatables (.anim)
class Video {
    private:

    public:
        static void compile(std::string filename, int);
};

#endif