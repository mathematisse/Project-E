#pragma once

#include <cstddef>
#include <span>
#include <concepts>
#include <vector>
#include <cstdint>

#include "lib_net/io/Result.hpp"

namespace lnet::io {

class Read {
public:
    virtual ~Read() = default;

    // Pull some bytes from this source into the specified buffer, returning
    // how many bytes were read.
    [[nodiscard]] virtual Result<std::size_t> read(const std::span<std::uint8_t> &buf) = 0;

    // Like `read`, except that it reads into a slice of buffers.
    [[nodiscard]] virtual Result<std::size_t>
    read_vectored(std::span<std::span<std::uint8_t>> bufs, size_t &bytesRead);

    // Determines if this `Read`er has an efficient `read_vectored` implementation.
    [[nodiscard]] virtual bool is_read_vectored() const { return false; }

    // Read all bytes until EOF in this source, placing them into `buf`.
    [[nodiscard]] virtual Result<std::size_t>
    read_to_end(std::vector<std::uint8_t> &buf, size_t &bytesRead);

    // Read all bytes until EOF in this source, appending them to `buf`.
    [[nodiscard]] virtual Result<std::size_t> read_to_string(std::string &buf, size_t &bytesRead);

    // Read the exact number of bytes required to fill `buf`. (return an error if EOF is reached)
    // the value of the success can be ignored, as the buffer will be filled with the exact number
    // of bytes
    virtual Result<std::size_t> read_exact(std::span<std::uint8_t> buf);
};

// Concept to check if a type T satisfies the Read interface
template<typename T>
concept Readable = requires(
    T t, std::span<std::uint8_t> buf, size_t bytesRead, std::span<std::span<std::uint8_t>> vec,
    std::vector<std::uint8_t> v, std::string str
) {
    { t.read(buf) } -> std::same_as<Result<std::size_t>>;
    { t.read_vectored(vec, bytesRead) } -> std::same_as<Result<std::size_t>>;
    { t.read_to_end(v, bytesRead) } -> std::same_as<Result<std::size_t>>;
    { t.read_to_string(str, bytesRead) } -> std::same_as<Result<std::size_t>>;
    { t.read_exact(buf) } -> std::same_as<Result<std::size_t>>;
};

// check if Read class satisfies the Readable concept
static_assert(Readable<Read>);

} // namespace lnet::io
