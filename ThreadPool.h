//
// Created by Jaskaran Singh Puri on 13.02.26.
//

#ifndef HTTPSERVER_THREADPOOL_H
#define HTTPSERVER_THREADPOOL_H

#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include <condition_variable>
#include <mutex>

class ThreadPool {
    public:
        ThreadPool(size_t num_threads) : stop(false) {

            for (size_t i = 0; i < num_threads; i++) {
                workers.emplace_back([this]() { this->worker_loop(); });
            }

        };
        ~ThreadPool();

        void enqueue(std::function<void()> task);

    private:
        std::vector<std::thread> workers;
        std::queue<std::function<void()>> tasks;

        std::mutex queue_mutex;
        std::condition_variable condition;
        bool stop;

        void worker_loop() {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(queue_mutex);

                    while (!stop && tasks.empty()) {
                        condition.wait(lock);
                    }

                    if (stop && tasks.empty()) {
                        return;
                    }

                    task = tasks.front();
                    tasks.pop();
                }
                task();
            }
        }
};

#endif //HTTPSERVER_THREADPOOL_H