// module-1-tests.cpp
// 10/14/24

#include "../include/StaticCanvas.h"
#include <cassert>

int main(int argc, char** argv) {
    // test: create a blank canvas of width 40 height 8
    StaticCanvas my_canvas(40, 6, 16);
    my_canvas.write_stcan("mycanvas.stcan");
}