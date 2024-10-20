

#pragma once

#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>

namespace net {

template<typename T>
class TSQueue {
public:
    TSQueue() = default;
    TSQueue(const TSQueue<T> &other) = delete;
    virtual ~TSQueue() { clear(); }

    const TSQueue<T> &operator=(const TSQueue<T> &other) = delete;

    void push(const T &item)
    {
        std::scoped_lock lock(mtx);
        queue.push(item);
        cv.notify_one();
    }

    void push(T &&item)
    {
        std::scoped_lock lock(mtx);
        queue.push(std::move(item));
        cv.notify_one();
    }

    template<typename... Args>
    void emplace(Args &&...args)
    {
        std::scoped_lock lock(mtx);
        queue.emplace(std::forward<Args>(args)...);
        cv.notify_one();
    }

    bool empty() const
    {
        std::scoped_lock lock(mtx);
        return queue.empty();
    }

    size_t count() const
    {
        std::scoped_lock lock(mtx);
        return queue.size();
    }

    bool try_pop(T &item)
    {
        std::scoped_lock lock(mtx);
        if (queue.empty()) {
            return false;
        }
        item = std::move(queue.front());
        queue.pop();
        return true;
    }

    std::optional<T> wait_and_pop()
    {
        std::unique_lock lock(mtx);
        cv.wait(lock, [this] {
            return !queue.empty();
        });
        T item = std::move(queue.front());
        queue.pop();
        return std::make_optional(item);
    }

    void clear()
    {
        std::scoped_lock lock(mtx);
        while (!queue.empty()) {
            queue.pop();
        }
    }

private:
    mutable std::mutex mtx;
    std::queue<T> queue;
    std::condition_variable cv;
};
}