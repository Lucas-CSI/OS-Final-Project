#include "threadpool.h"

using namespace std;

ThreadPool::ThreadPool(size_t num_threads, size_t queue_size)
    : task_queue_(queue_size), shutdown_(false) {
}

ThreadPool::~ThreadPool() {
}

void ThreadPool::worker_loop() {
}

bool ThreadPool::submit(function<void()> task) {
    return false;
}

void ThreadPool::shutdown() {
}
