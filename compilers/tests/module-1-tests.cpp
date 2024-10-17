// module-1-tests.cpp
// 10/14/24

#include "../include/StaticCanvas.h"
#include "../include/util.h"
#include <cassert>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <sstream>

using namespace std;

void create() {
    // test: create a blank canvas of width 40 height 6
    StaticCanvas my_canvas(40, 6, 16);
    my_canvas.write_stcan("mycanvas.stcan");
}

void compile_and_copy() {
    try {
        // test: compile terminux/examples/giveup.stcan
        StaticCanvas *giveup = StaticCanvas::read_stcan("../../examples/giveup.stcan");
        giveup->compile("giveup.anim", 20);
        giveup->write_stcan("giveup_copy.anim");
        delete giveup;
    } catch (string e) {
        cerr << e << endl;
    }
}

void test_util_get_section() {
    string metadata = get_section("../../examples/giveup.stcan", "metadata");
    assert(metadata == 
    "Frame Width: 40\n"
    "Frame Height: 6\n"
    "Number of Frames: 16\n");

    string comments = get_section("../../examples/giveup.stcan", "comments");
    assert(comments == 
    "\n"
    "Static Canvas: giveup.stcan\n"
    "\n"
    "This is a Static Canvas. In this file, you can design ASCII animations with pixel-perfect accuracy!\n"
    "Create frames by editing the text segments within the rectangular borders in the frames section below.\n"
    "Ensure that the frame borders remain aligned according to the width and height of the canvas.\n"
    "When you're ready, compile using:\n"
    "terminux giveup.stcan\n"
    "You can use this commment space to write personal notes or descriptions for your animations.\n"
    "The sections in this file are sectioned by \\section and \\end labels.\n"
    "Please do not modify anything within the metadata section.\n"
    "Within the frames section, ONLY modify the content within the frame borders.\n"
    "\n\n\n");

    cout << "ASSERTIONS PASSED: test_util_get_section()" << endl;
}

void test_scale_resize() {
    vector<vector<char>> sample{{'a', 'b', 'c'}, {'d', 'e', 'f'}};
    Frame f1{sample};
    f1.scale(6, 4);
    ostringstream temp{};
    temp << f1;
    assert(temp.str() == "aabbcc\naabbcc\nddeeff\nddeeff");

    Frame f2{sample};
    f2.scale(2, 3);
    temp.str(""); // clears
    temp << f2;
    assert(temp.str() == "ab\nab\nde");

    vector<vector<char>> sample2{{'a', 'b', 'c', 'd'}, {'e', 'f', 'g', 'h'},
            {'i', 'j', 'k', 'l'}};
    Frame f3{sample2};
    f3.scale(6, 7);
    temp.str("");
    temp << f3;
    assert(temp.str() == 
    "aabccd\n"
    "aabccd\n"
    "aabccd\n"
    "eefggh\n"
    "eefggh\n"
    "iijkkl\n"
    "iijkkl"
    );

    Frame f4{sample2};
    f4.resize(3, 2);
    temp.str("");
    temp << f4;
    assert(temp.str() == "abc\nefg");

    Frame f5{sample2};
    f5.resize(6, 6);
    temp.str("");
    temp << f5;
    assert(temp.str() == "abcd  \nefgh  \nijkl  \n      \n      \n      ");

    cout << "ASSERTIONS PASSED: FRAME SCALE AND RESIZE" << endl;
}

int main(int argc, char** argv) {
    // test_util_get_section();
    // compile_and_copy();
    test_scale_resize();
}