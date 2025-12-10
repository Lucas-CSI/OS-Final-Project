#include "threadpool.h"
#include <iostream>

// Constructor: launches a given number of worker threads
ThreadPool::ThreadPool(size_t num_threads) : stop(false) {
    for (size_t i = 0; i < num_threads; ++i) {
        workers.emplace_back(&ThreadPool::worker_thread, this);
    }
}

// Destructor: joins all threads
ThreadPool::~ThreadPool() {
    {
        // Lock the mutex to safely change the 'stop' flag
        unique_lock<mutex> lock(queue_mutex);
        stop = true;
    }

    // Wake up all threads so they can see the 'stop' flag
    condition.notify_all();

    // Wait for all threads to finish their execution
    for (thread &worker : workers) {
        worker.join();
    }
}

// The function that each worker thread runs in a loop
void ThreadPool::worker_thread() {
    while (true) {
        function<void()> task;

        {
            // Lock the mutex and wait for a task or a stop signal
            unique_lock<mutex> lock(queue_mutex);
            
            condition.wait(lock, [this] {
                return stop || !tasks.empty();
            });

            // If the pool is stopped and there are no more tasks, exit the thread
            if (stop && tasks.empty()) {
                return;
            }

            // Get the next task from the queue
            task = move(tasks.front());
            tasks.pop();
        }

        task();
    }
}
