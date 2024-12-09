#include <iostream>
#include <fstream>

using namespace std;

// 2 threads 
// - read (and decompress...work on that later)
// - write

// implement using deque
// - read_thread will read and decompress a line

int main(int argc, char **argv) {
  if (argc != 2) {
    cout << "Usage: ./exec [file.anim]" << endl;
    return -1;
  }

  ifstream ifs{argv[1]};

  //for (int ) {}
}