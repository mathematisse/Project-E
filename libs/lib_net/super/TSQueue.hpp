#pragma once

#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>

namespace lnet::utils {

/// example usage using cosumer-producer pattern
/// {
///     TSQueue<int> queue;
///     std::thread producer([&queue] {
///         for (int i = 0; i < 100; ++i) {
///             queue.push(i);
///         }
///     });
///     std::thread consumer([&queue] {
///         for (int i = 0; i < 100; ++i) {
///             auto value = queue.wait_pop();
///             if (value) {
///                 std::cout << *value << std::endl;
///             }
///         }
///     });
///     producer.join();
///     consumer.join();
/// }
template<typename T>
class TsQueue {
public:
    TsQueue() = default;
    ~TsQueue() { stop(); }

private:
    std::queue<T> _queue;
    mutable std::mutex _mutex;
    std::condition_variable _condition;
    bool _stopped {};

public:
    [[nodiscard]] inline auto size() const -> size_t
    {
        std::scoped_lock lock(_mutex);
        return _queue.size();
    }

    void push(const T &value)
    {
        {
            std::scoped_lock lock(_mutex);
            _queue.push(value);
        }
        _condition.notify_one();
    }

    void push(T &&value)
    {
        {
            std::scoped_lock lock(_mutex);
            _queue.push(std::move(value));
        }
        _condition.notify_one();
    }

    [[nodiscard]] auto wait_pop() -> std::optional<T>
    {
        std::unique_lock lock(_mutex);
        _condition.wait(lock, [this] {
            return !_queue.empty() || _stopped;
        });
        if (_stopped && _queue.empty()) {
            return std::nullopt;
        }
        auto value = _queue.front();
        _queue.pop();
        return value;
    }

    [[nodiscard]] auto wait_pop(std::chrono::milliseconds timeout) -> std::optional<T>
    {
        std::unique_lock lock(_mutex);
        if (!_condition.wait_for(lock, timeout, [this] {
                return !_queue.empty() || _stopped;
            })) {
            return std::nullopt;
        }
        if (_stopped && _queue.empty()) {
            return std::nullopt;
        }
        auto value = _queue.front();
        _queue.pop();
        return value;
    }

    [[nodiscard]] auto try_pop() -> std::optional<T>
    {
        std::scoped_lock lock(_mutex);
        if (_queue.empty()) {
            return std::nullopt;
        }
        auto value = _queue.front();
        _queue.pop();
        return value;
    }

    void clear()
    {
        std::scoped_lock lock(_mutex);
        while (!_queue.empty()) {
            _queue.pop();
        }
    }

    [[nodiscard]] auto empty() const -> bool
    {
        std::scoped_lock lock(_mutex);
        return _queue.empty();
    }

    void stop()
    {
        {
            std::scoped_lock lock(_mutex);
            _stopped = true;
        }
        _condition.notify_all();
    }

    [[nodiscard]] auto is_stopped() const -> bool
    {
        std::scoped_lock lock(_mutex);
        return _stopped;
    }
};

} // namespace lnet::utils