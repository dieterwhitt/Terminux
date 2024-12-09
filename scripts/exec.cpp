#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <deque>
#include <cstdlib>
#include <memory>

using namespace std;

// 2 threads 
// - read (and decompress...work on that later)
// - write

// implement using deque
// - read_thread will read and decompress a line

// https://www.geeksforgeeks.org/std-mutex-in-cpp/

// number of frames that will be loaded into memory at a MAXIMUM
const int BUFFER_SIZE = 100;

// time in ms the read/write thread will wait if buffer full or empty, respectively
const int BUFFER_DELAY_MS = 1; 

// buffer of frames to be loaded into memory for smooth framerate
deque<string> buffer(1);

// locks for each buffer frame to prevent race conditions
deque<unique_ptr<mutex>> buffer_locks(1);

void readFrame(ifstream &ifs, int y_res) {
    while (!ifs.eof()) {
        // wait until 
        while (buffer_locks.size() >= BUFFER_SIZE) {
            this_thread::sleep_for(chrono::microseconds(BUFFER_DELAY_MS));
        }

        string frame = "";
        string row;
        // read a single frame at once
        for (int y = 0; y < y_res; ++y) {
            getline(ifs, row);
            if (!ifs.eof()) {
                frame += (row + '\n');
            } else {
                return; // kys
            }
        }
        // mutex added to back locked

        unique_ptr<mutex> mtx = make_unique<mutex>();
        mtx->lock();
        buffer_locks.push_back(std::move(mtx));

        // push to frame buffer
        buffer.push_back(std::move(frame));

        buffer_locks.back()->unlock();
        // end of frame
        // clear console
    }
}

void writeFrame(ostream &out, int y_res, int numframes) {
    int cnt = 0;
    while (cnt < numframes) {
        while (buffer_locks.size() == 0) {
            this_thread::sleep_for(chrono::microseconds(BUFFER_DELAY_MS));
        }

        buffer_locks.front()->lock();
        cout << buffer.front();
        buffer_locks.front()->unlock();
        buffer.pop_front();

        system("clear");
        //cout << "\033[2J";
    }
}

int main(int argc, char **argv) {
    // ensure that argument is added
    if (argc != 2) {
        cout << "Usage: ./exec [filename.anim]" << endl;
        return -1;
    }

    // open read stream
    ifstream ifs{argv[1]};

    // read metadata from file
    int framerate, num_frames, x_res, y_res, loop;
    ifs >> framerate >> num_frames >> x_res >> y_res >> loop;

    // initialize read/write threads
    thread read_thread(readFrame, ref(ifs), y_res);
    thread write_thread(writeFrame, ref(cout), y_res, num_frames);

    do {
        // start threads
        read_thread.join();
        // include delay time between thread starts?
        write_thread.join();
    } while (loop);
}