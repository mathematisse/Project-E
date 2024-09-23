#pragma once

#include <condition_variable>
#include <deque>
#include <mutex>

namespace net {
template<typename T>
class TSQueue {
public:
    TSQueue() = default;
    TSQueue(const TSQueue<T> &) = delete;
    TSQueue &operator=(const TSQueue<T> &) = delete;
    TSQueue(TSQueue<T> &&) = default;
    TSQueue &operator=(TSQueue<T> &&) = default;
    virtual ~TSQueue() = default;

public:
    const T &front()
    {
        std::scoped_lock lock(muxQueue);
        return deqQueue.front();
    }

    const T &back()
    {
        std::scoped_lock lock(muxQueue);
        return deqQueue.back();
    }

    T pop_front()
    {
        std::scoped_lock lock(muxQueue);
        auto data = std::move(deqQueue.front());
        deqQueue.pop_front();
        return data;
    }

    T pop_back()
    {
        std::scoped_lock lock(muxQueue);
        auto data = std::move(deqQueue.back());
        deqQueue.pop_back();
        return data;
    }

    void push_back(const T &item)
    {
        std::scoped_lock lock(muxQueue);
        deqQueue.emplace_back(std::move(item));

        std::unique_lock<std::mutex> unique_lock(muxBlocking);
        cvBlocking.notify_one();
    }

    void push_front(const T &item)
    {
        std::scoped_lock lock(muxQueue);
        deqQueue.emplace_front(std::move(item));

        std::unique_lock<std::mutex> unique_lock(muxBlocking);
        cvBlocking.notify_one();
    }

    bool empty()
    {
        std::scoped_lock lock(muxQueue);
        return deqQueue.empty();
    }

    size_t count()
    {
        std::scoped_lock lock(muxQueue);
        return deqQueue.size();
    }

    void clear()
    {
        std::scoped_lock lock(muxQueue);
        deqQueue.clear();
    }

    void wait()
    {
        while (empty()) {
            std::unique_lock<std::mutex> lock(muxBlocking);
            cvBlocking.wait(lock);
        }
    }

protected:
    std::mutex muxQueue;
    std::deque<T> deqQueue;
    std::condition_variable cvBlocking;
    std::mutex muxBlocking;
};
}
