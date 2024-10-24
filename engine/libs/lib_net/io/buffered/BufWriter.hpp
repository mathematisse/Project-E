
#pragma once

#include <cstddef>
#include <vector>

#include "lib_net/io/Write.hpp"
#include "lib_net/io/buffered/Buffer.hpp"
#include "lib_net/io/Result.hpp"

namespace lnet::io {

class BufWriter {
private:
    std::vector<std::byte> _buffer;
    Write &_inner;

public:
    explicit BufWriter(Write &inner):
        _inner(inner)
    {
        _buffer.reserve(DEFAULT_BUF_SIZE);
    }
    BufWriter(Write &inner, size_t size):
        _buffer(size),
        _inner(inner)
    {
    }

    [[nodiscard]] auto writer() const -> const Write & { return _inner; }
    [[nodiscard]] auto buffer() const -> const std::vector<std::byte> & { return _buffer; }
    [[nodiscard]] auto capacity() const -> size_t { return _buffer.capacity(); }
    [[nodiscard]] auto len() const -> size_t { return _buffer.size(); }
    [[nodiscard]] auto is_empty() const -> bool { return _buffer.empty(); }

private:
    size_t write_to_buf(const std::span<std::byte> &buf)
    {
        size_t available = _buffer.capacity() - _buffer.size();
        size_t amt_to_buffer = std::min(available, buf.size());

        _buffer.insert(_buffer.end(), buf.begin(), buf.begin() + amt_to_buffer);

        return amt_to_buffer;
    }

    auto write_cold(const std::span<std::byte> &buf) -> Result<size_t>
    {
        if (buf.size() > (_buffer.capacity() - _buffer.size())) {
            auto flush_result = flush_buf();
            if (!flush_result) {
                return Result<size_t>::Error(flush_result);
            }
        }

        if (buf.size() >= _buffer.capacity()) {
            auto write_result = _inner.write(buf);
            if (!write_result) {
                return write_result;
            }
            return Result<size_t>(buf.size());
        } else {
            _buffer.insert(_buffer.end(), buf.begin(), buf.end());
            return Result<size_t>(buf.size());
        }
    }

    auto write_all_cold(const std::span<std::byte> &buf) -> Result<result::Void>
    {
        if (buf.size() > (_buffer.capacity() - _buffer.size())) {
            auto flush_result = flush_buf();
            if (!flush_result) {
                return flush_result;
            }
        }

        if (buf.size() >= _buffer.capacity()) {
            auto write_result = _inner.write_all(buf);
            if (!write_result) {
                return write_result;
            }
        } else {
            _buffer.insert(_buffer.end(), buf.begin(), buf.end());
        }

        return Result<result::Void>(result::Void {});
    }

public:
    auto write(const std::span<std::byte> &buf) -> Result<size_t>
    {
        if (buf.size() < (_buffer.capacity() - _buffer.size())) {
            _buffer.insert(_buffer.end(), buf.begin(), buf.end());
            return Result<size_t>(buf.size());
        } else {
            return write_cold(buf);
        }
    }

    auto write_all(const std::span<std::byte> &buf) -> Result<result::Void>
    {
        if (buf.size() < (_buffer.capacity() - _buffer.size())) {
            _buffer.insert(_buffer.end(), buf.begin(), buf.end());
            return Result<result::Void>(result::Void {});
        } else {
            return write_all_cold(buf);
        }
    }

    auto write_vectored(const std::vector<std::span<std::byte>> &bufs) -> Result<size_t>
    {
        size_t total_len = 0;
        for (const auto &buf : bufs) {
            total_len += buf.size();
            if (total_len > _buffer.capacity() - _buffer.size() && !_buffer.empty()) {
                auto flush_result = flush_buf();
                if (!flush_result) {
                    return Result<std::size_t>::Error(flush_result);
                }
            }
            if (total_len >= _buffer.capacity()) {
                for (const auto &buf : bufs) {
                    auto write_result = _inner.write(buf);
                    if (!write_result) {
                        return write_result;
                    }
                }
                return Result<size_t>(total_len);
            }
        }

        for (const auto &buf : bufs) {
            _buffer.insert(_buffer.end(), buf.begin(), buf.end());
        }

        return Result<size_t>::Success(total_len);
    }

    [[nodiscard]] static bool is_write_vectored() { return true; }

    auto flush() -> Result<result::Void>
    {
        auto flush_result = flush_buf();
        if (!flush_result) {
            return flush_result;
        }
        return _inner.flush();
    }

private:
    auto flush_buf() -> Result<result::Void>
    {
        if (_buffer.empty()) {
            return Result<result::Void>(result::Void {});
        }

        auto write_result = _inner.write_all(_buffer);
        if (!write_result) {
            return write_result;
        }

        _buffer.clear();
        return Result<result::Void>(result::Void {});
    }
};

} // namespace lnet::io