#ifndef BLOCKING_QUEUE_H
#define BLOCKING_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <stdexcept>

using namespace std;

template<typename T>
class BlockingQueue {
private:
    queue<T> queue_;
    mutable mutex mutex_;
    condition_variable not_empty_;
    condition_variable not_full_;
    size_t max_size_;

public:
    explicit BlockingQueue(size_t max_size) : max_size_(max_size) {
        if (max_size == 0) {
            throw invalid_argument("Queue size must be greater than 0");
        }
    }

    void push(const T& item) {
        unique_lock<mutex> lock(mutex_);
        not_full_.wait(lock, [this] { return queue_.size() < max_size_; });
        queue_.push(item);
        not_empty_.notify_one();
    }

    bool try_push(const T& item) {
        unique_lock<mutex> lock(mutex_);
        if (queue_.size() >= max_size_) {
            return false;
        }
        queue_.push(item);
        not_empty_.notify_one();
        return true;
    }

    T pop() {
        unique_lock<mutex> lock(mutex_);
        not_empty_.wait(lock, [this] { return !queue_.empty(); });
        T item = queue_.front();
        queue_.pop();
        not_full_.notify_one();
        return item;
    }

    bool empty() const {
        lock_guard<mutex> lock(mutex_);
        return queue_.empty();
    }

    size_t size() const {
        lock_guard<mutex> lock(mutex_);
        return queue_.size();
    }

    size_t max_size() const {
        return max_size_;
    }
};

#endif // BLOCKING_QUEUE_H
