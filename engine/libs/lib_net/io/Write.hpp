#pragma once

#include <concepts>
#include <span>
#include <string>

#include "lib_net/io/Result.hpp"
#include "lib_net/result/Result.hpp"

namespace net::io {

class Write {
public:
    virtual ~Write() = default;

    // Write a buffer into this writer, returning how many bytes were written.
    virtual Result<size_t> write(const std::span<std::byte> &buf) = 0;

    // Flush this output stream, ensuring that all intermediately buffered contents reach their
    // destination.
    virtual Result<result::Void> flush() = 0;

    // Like write, except that it writes from a slice of buffers.
    virtual Result<size_t> write_vectored(const std::span<std::span<std::byte>> &bufs);

    // Determines if this writer has an efficient write_vectored implementation.
    [[nodiscard]] virtual bool is_write_vectored() const { return false; }

    // Attempts to write an entire buffer into this writer.
    Result<result::Void> write_all(const std::span<std::byte> &buf);

    // Attempts to write multiple buffers into this writer.
    Result<result::Void> write_all_vectored(std::span<std::span<std::byte>> &bufs);

    // Writes a string into this writer, returning any error encountered.
    Result<result::Void> write_str(const std::string &str);
};

// Concept to check if a type T satisfies the Write interface
template<typename T>
concept Writable = requires(T t, const std::span<std::byte> &buf) {
    { t.write(buf) } -> std::same_as<Result<size_t>>;
    { t.write_vectored(buf) } -> std::same_as<Result<size_t>>;
    { t.flush() } -> std::same_as<Result<result::Void>>;
};

} // namespace net::io