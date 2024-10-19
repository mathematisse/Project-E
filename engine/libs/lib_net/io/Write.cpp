
#pragma once

#include <algorithm>
#include <span>
#include <vector>
#include <string>

#include "lib_net/io/Result.hpp"
#include "lib_net/result/Result.hpp"
#include "lib_net/io/Write.hpp"

namespace net::io {

Result<size_t> Write::write_vectored(const std::span<std::span<std::byte>> &bufs)
{
    size_t total_written = 0;
    for (const auto &buf : bufs) {
        auto result = write(buf);
        if (!result) {
            return result;
        }
        total_written += result.value();
    }
    return Result<size_t>(total_written);
}

Result<result::Void> Write::write_all(const std::span<std::byte> &buf)
{
    size_t total_written = 0;
    while (total_written < buf.size()) {
        auto result = write({buf.begin() + total_written, buf.end()});
        if (!result) {
            return Result<result::Void>(result.error());
        }
        total_written += result.value();
    }
    return Result<result::Void>(result::Void {});
}

Result<result::Void> Write::write_all_vectored(std::span<std::span<std::byte>> &bufs)
{
    for (auto &buf : bufs) {
        auto result = write_all(buf);
        if (!result) {
            return result;
        }
    }
    return Result<result::Void>(result::Void {});
}

Result<result::Void> Write::write_str(const std::string &str)
{
    std::vector<std::byte> byte_buf(str.size());
    // Convert the string to a byte buffer
    std::transform(str.begin(), str.end(), byte_buf.begin(), [](char c) {
        return std::byte(c);
    });
    return write_all(byte_buf);
}

} // namespace net::io