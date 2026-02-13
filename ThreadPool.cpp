//
// Created by Jaskaran Singh Puri on 13.02.26.
//

#include "ThreadPool.h"
#include <iostream>

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }

    condition.notify_all();

    for (std::thread& worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }

    std::cout << "ThreadPool destroyed, all threads joined." << std::endl;
}

void ThreadPool::enqueue(std::function<void()> task) {
    std::unique_lock<std::mutex> lock(queue_mutex);
    if (!stop) {
        tasks.push(task);
    }
    condition.notify_one();
}
