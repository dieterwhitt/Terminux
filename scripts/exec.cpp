#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <deque>
#include <cstdlib>
#include <memory>
#include <ctime>

#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>

using namespace std;

// 2 threads 
// - read (and decompress...work on that later)
// - write

// implement using deque
// - read_thread will read and decompress a line

// https://www.geeksforgeeks.org/std-mutex-in-cpp/

// number of frames that will be loaded into memory at a MAXIMUM
const int BUFFER_TIME = 10;

// time in ms the read/write thread will wait if buffer full or empty, respectively
const int BUFFER_DELAY_US = 1; 

// buffer of frames to be loaded into memory for smooth framerate
deque<string> buffer(1);

// locks for each buffer frame to prevent race conditions
deque<unique_ptr<mutex>> buffer_locks;

void clearConsole() {
    /*
    // struct winsize w;
    // ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); // get terminal size
    // for (int i = 0; i < w.ws_row; ++i) {
    //     printf("\n");
    //     //std::cout << "\n";
    // }
    //std::cout.flush();
    //std::cout << "\033[2J\033[H";
    */
    printf("\033[2J\033[H");
}

inline void precise_sleep(long long duration_us) {
    auto start = std::chrono::high_resolution_clock::now();
    auto end = start + std::chrono::microseconds(duration_us);
    while (std::chrono::high_resolution_clock::now() < end) {
        // busy-wait loop
    }
}

void readFrame(ifstream &ifs, int y_res, int framerate) {
    while (!ifs.eof()) {
        // wait until 
        while (buffer_locks.size() >= BUFFER_TIME * framerate) {
            this_thread::sleep_for(chrono::microseconds(BUFFER_DELAY_US));
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

void writeFrame(ostream &out, int y_res, int numframes, int framerate) {
    int frames_read = 0;
    long long total_sleep_time_us = 0;
    long long total_read_time_us = 0;

    long long frame_delay_us = static_cast<long long>(1e6 / framerate);

    while (frames_read < numframes) {
        while (buffer_locks.size() == 0) {
            this_thread::sleep_for(chrono::microseconds(BUFFER_DELAY_US));
        }

        auto start = std::chrono::high_resolution_clock::now();

        // lock front, print, unlock
        buffer_locks.front()->lock();
        clearConsole();
        printf("%s", buffer.front().c_str());
        buffer_locks.front()->unlock();
        // pop buffer and mutex front
        buffer.pop_front();
        buffer_locks.pop_front();

        auto stop = std::chrono::high_resolution_clock::now();
        // Calculate the duration
        long long duration = std::chrono::duration_cast<std::chrono::microseconds>
                (stop - start).count();

        cerr << "[READER] time spent printing and clearing a frame: " << 
                duration << "us" << endl;


        auto start_sleep = std::chrono::high_resolution_clock::now();

        // sleep for 1 frame
        // this_thread::sleep_for(chrono::microseconds(frame_delay_us));
        precise_sleep(frame_delay_us);

        auto stop_sleep = std::chrono::high_resolution_clock::now();

        long long duration_sleep = std::chrono::duration_cast<std::chrono::microseconds>
                (stop_sleep - start_sleep).count();

        cerr << "[READER] extra time spent sleeping on a frame: " << 
                duration_sleep - frame_delay_us << "us" << endl;

        ++frames_read;
        total_read_time_us += duration;
        total_sleep_time_us += (duration_sleep - frame_delay_us);
    }
    cerr << "[READER] time spent printing and clearing all frames: " << 
        total_read_time_us << "us" << endl;
    cerr << "[READER] extra time spent sleeping: " << 
        total_sleep_time_us << "us" << endl;
    
}

int main(int argc, char **argv) {
    buffer_locks.push_back(make_unique<mutex>());

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
    thread read_thread(readFrame, ref(ifs), y_res, framerate);
    thread write_thread(writeFrame, ref(cout), y_res, num_frames, framerate);

    time_t start = time(NULL);

    do {
        // start threads
        read_thread.join();
        // include delay time between thread starts?
        write_thread.join();
    } while (loop);

    cout << time(NULL) - start << endl;
}