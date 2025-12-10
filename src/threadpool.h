#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>

using namespace std;

class ThreadPool {
public:
    // Constructor: creates a pool of worker threads
    ThreadPool(size_t num_threads);

    // Destructor: joins all threads to ensure clean shutdown
    ~ThreadPool();

    template<class F, class... Args>
    auto submit(F&& f, Args&&... args) -> future<decltype(f(args...))>;

private:
    // The function that each worker thread executes
    void worker_thread();

    vector<thread> workers;

    // The task queue
    queue<function<void()>> tasks;

    // Synchronization
    mutex queue_mutex;
    condition_variable condition;

    // Flag to stop the threads
    bool stop;
};

template<class F, class... Args>
auto ThreadPool::submit(F&& f, Args&&... args) -> future<decltype(f(args...))> {
    // Deduce the return type of the function
    using return_type = decltype(f(args...));

    // Create a task, which is a wrapper that will hold the function and its arguments.
    auto task = make_shared<packaged_task<return_type()>>(
        bind(forward<F>(f), forward<Args>(args)...)
    );

    // Get the future associated with the task to return to the caller
    future<return_type> res = task->get_future();

    {
        // Lock the queue to add the new task
        unique_lock<mutex> lock(queue_mutex);

        // Don't allow submitting new tasks after the pool is stopped
        if (stop) {
            throw runtime_error("submit on stopped ThreadPool");
        }

        tasks.emplace([task](){ (*task)(); });
    }

    // Notify one waiting thread that a new task is available
    condition.notify_one();

    return res;
}

#endif // THREADPOOL_H
