#include "ThreadPool.h"
#include <iostream>
#include <chrono>

void complex_calculation(int id) {
    std::cout << "Task " << id << " starting..." << std::endl;
    // Simulate hard work (sleep for 1 second)
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Task " << id << " COMPLETED." << std::endl;
}

int main() {
    ThreadPool pool(4); // Create 4 workers

    // Queue 8 tasks
    for(int i = 0; i < 8; ++i) {
        pool.enqueue([i] {
            complex_calculation(i);
        });
    }

    // Main thread waits a bit to let them finish
    std::this_thread::sleep_for(std::chrono::seconds(3));
    
    return 0;
}