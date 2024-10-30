#pragma once

#include <algorithm>
#include <mutex>

namespace lnet::io {

template<typename T>
class Mutex {

public:
    explicit Mutex(T data):
        _data(std::move(data))
    {
    }
    Mutex(const Mutex &) = delete;
    Mutex &operator=(const Mutex &) = delete;

    void lock() { _mutex.lock(); }
    void unlock() { _mutex.unlock(); }
    bool try_lock() { return _mutex.try_lock(); }

    template<typename F>
    auto with_lock(F &&f) -> decltype(f(std::declval<T &>()))
    {
        std::lock_guard lock(_mutex);
        return f(_data);
    }

private:
    T _data;
    std::mutex _mutex;
};

} // namespace lnet::io