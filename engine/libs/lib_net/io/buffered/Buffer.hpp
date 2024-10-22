#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <span>

#include "lib_net/io/Read.hpp"
#include "lib_net/io/Result.hpp"

namespace lnet::io {

constexpr size_t DEFAULT_BUF_SIZE = 8192;
// Buffer class with fixed-size buffer and manual allocation
class Buffer {
private:
    std::size_t _pos = 0;
    std::size_t _filled = 0;
    std::size_t _capacity;
    std::byte *_buffer;

public:
    explicit Buffer(std::size_t capacity):
        _capacity(capacity),
        _buffer(new std::byte[capacity]) {};

    ~Buffer() { delete[] _buffer; }

    Buffer(const Buffer &other) = delete;
    Buffer &operator=(const Buffer &other) = delete;
    Buffer(Buffer &&other) noexcept = default;
    Buffer &operator=(Buffer &&other) noexcept = default;

    inline auto buffer() const -> std::span<std::byte>
    {
        return std::span<std::byte>(_buffer, _filled).subspan(_pos);
    }

    [[nodiscard]] inline auto capacity() const -> std::size_t { return _capacity; }
    [[nodiscard]] inline auto filled() const -> std::size_t { return _filled; }
    [[nodiscard]] inline auto pos() const -> std::size_t { return _pos; }

    inline auto discard_buffer() -> void
    {
        _pos = 0;
        _filled = 0;
    }

    inline auto consume(std::size_t amt) -> void { _pos = std::min(_pos + amt, _filled); }

    template<typename V>
    inline auto consume_with(std::size_t amt, V &&visitor) -> bool
    {
        if (_filled - _pos < amt) {
            return false;
        }

        visitor(std::span<std::byte>(_buffer, _filled).subspan(_pos, amt));
        consume(amt);
        return true;
    }

    inline auto unconsume(std::size_t amt) -> void { _pos = std::max(_pos - amt, 0UL); }

    template<typename R>
        requires io::Readable<R>
    inline auto fill_buf(R &reader) -> io::Result<std::span<std::byte>>
    {
        if (_pos == _filled) {
            _pos = 0;
            _filled = 0;
        }

        if (_filled == _capacity) {
            return io::Result<std::span<std::byte>>(std::span<std::byte>());
        }

        auto buf = std::span<std::byte>(_buffer, _capacity).subspan(_filled);
        size_t bytesRead = 0;
        auto error_code = reader.read(buf, bytesRead);
        if (error_code) {
            return error_code;
        }
        _filled += bytesRead;
        auto rem = std::span<std::byte>(_buffer, _filled).subspan(_filled - bytesRead, bytesRead);
        return io::Result<std::span<std::byte>>(rem);
    }
};
}