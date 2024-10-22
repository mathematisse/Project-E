
#pragma once

#include "lib_net/io/Result.hpp"
#include "lib_net/io/buffered/Buffer.hpp"
#include "lib_net/result/Result.hpp"
#include <cstddef>
#include <span>
#include <variant>

namespace lnet::io {

template<typename R>
class BufReader {
private:
    Buffer _buffer;
    R _inner;

public:
    explicit BufReader(R inner):
        _inner(std::move(inner)),
        _buffer(DEFAULT_BUF_SIZE)
    {
    }
    BufReader(R inner, size_t size):
        _inner(std::move(inner)),
        _buffer(size)
    {
    }

    [[nodiscard]] auto reader() const -> const R & { return _inner; }
    [[nodiscard]] auto buffer() const -> std::span<std::byte> { return _buffer.buffer(); }
    [[nodiscard]] auto capacity() const -> size_t { return _buffer.capacity(); }
    auto discard_buffer() -> void { _buffer.discard_buffer(); }

    auto fill_buf() -> Result<std::span<std::byte>> { return _buffer.fill_buf(_inner); }
    auto consume(size_t amt) -> void { _buffer.consume(amt); }

    // read implementation part
    auto read(std::span<std::byte> buf) -> Result<std::size_t>
    {
        if (_buffer.pos() == _buffer.filled() && buf.size() >= capacity()) {
            discard_buffer();
            return _inner.read(buf);
        }
        auto rem = fill_buf();
        if (!rem) {
            return rem.error();
        }
        auto nread = rem->read(buf);
        if (!nread) {
            return nread.error();
        }
        _buffer.consume(nread.value());
        return Result<std::size_t>::Success(nread.value());
    }

    auto read_exact(std::span<std::byte> buf) -> Result<result::Void>
    {
        size_t total_read = 0;
        while (total_read < buf.size()) {
            auto result = read(buf.subspan(total_read));
            if (!result) {
                return result.error();
            }
            total_read += result.value();
        }
        return Result<result::Void>::Success({});
    }

    auto read_vectored(std::span<std::span<std::byte>> bufs) -> Result<std::size_t>
    {
        size_t total_len = 0;
        for (const auto &buf : bufs) {
            total_len += buf.size();
        }
        if (_buffer.pos() == _buffer.filled() && total_len >= capacity()) {
            discard_buffer();
            return _inner.read_vectored(bufs);
        }
        auto rem = fill_buf();
        if (!rem) {
            return rem.error();
        }
        auto nread = rem->read_vectored(bufs);
        if (!nread) {
            return nread.error();
        }
        _buffer.consume(nread.value());
        return Result<std::size_t>::Success(nread.value());
    }

    [[nodiscard]] auto is_read_vectored() const -> bool { return _inner.is_read_vectored(); }

    auto read_to_end(std::vector<std::byte> &buf) -> Result<std::size_t>
    {
        auto inner_buf = _buffer.buffer();
        buf.reserve(buf.size() + inner_buf.size());
        buf.insert(buf.end(), inner_buf.begin(), inner_buf.end());
        auto nread = inner_buf.size();
        discard_buffer();
        auto result = _inner.read_to_end(buf);
        if (!result) {
            return result.error();
        }
        return Result<std::size_t>::Success(nread + result.value());
    }

    auto read_to_string(std::string &buf) -> Result<std::size_t>
    {
        if (buf.empty()) {
            std::vector<std::byte> byte_buf;
            auto result = read_to_end(byte_buf);
            if (!result) {
                return result.error();
            }
            buf.append(reinterpret_cast<const char *>(byte_buf.data()), byte_buf.size());
            return Result<std::size_t>::Success(result.value());
        } else {
            std::vector<std::byte> byte_buf;
            auto result = read_to_end(byte_buf);
            if (!result) {
                return result.error();
            }
            auto str =
                std::string(reinterpret_cast<const char *>(byte_buf.data()), byte_buf.size());
            buf.append(str);
            return Result<std::size_t>::Success(str.size());
        }
    }
};

} // namespace lnet::io