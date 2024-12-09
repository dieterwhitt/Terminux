#include <iostream>
#include <vector>
#include <mutex>
#include <thread>

using namespace std;

const int SIZE = 10;
vector<int> data(SIZE, 0);           // Shared data
vector<mutex> locks(SIZE);     // Mutexes for each element

void update_element(int index, int value) {
    // Lock the mutex for the specific index
    locks[index].lock();
    data[index] = value; // Critical section
    cout << "Updated index " << index << " to value " << value << "\n";
    locks[index].unlock();
}

void read_elem(int index) {
    locks[index].lock();
    cout << "Read val: " << data[index] << endl;
    locks[index].unlock();
}

int main() {
    thread t1(update_element, 3, 42);
    thread t2(update_element, 3, 99);
    thread t3(update_element, 7, 55);
    thread t4(read_elem, 3);

    t1.join();
    t4.join();
    t2.join();
    t3.join();

    return 0;
}
