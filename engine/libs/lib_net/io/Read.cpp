
#include <cstddef>

#include "Read.hpp"

namespace net::io {

// Like `read`, except that it reads into a slice of buffers.
Result<std::size_t> Read::read_vectored(std::span<std::span<std::byte>> bufs, size_t &bytesRead)
{
    size_t totalBytesRead = 0;
    for (auto &buf : bufs) {
        auto ec = read(buf);
        if (ec.isError()) {
            return io::Result<std::size_t>::Error(ec.error());
        }
        totalBytesRead += bytesRead;
        if (bytesRead < buf.size()) {
            break;
        }
    }
    return io::Result<std::size_t>::Success(totalBytesRead);
}

// Read all bytes until EOF in this source, placing them into `buf`.
Result<std::size_t> Read::read_to_end(std::vector<std::byte> &buf, size_t &bytesRead)
{
    size_t totalBytesRead = 0;
    std::span<std::byte> tempBuf(buf.data(), buf.size());
    while (true) {
        auto errorCode = read(tempBuf);
        if (errorCode.isError()) {
            return io::Result<std::size_t>::Error(errorCode.error());
        }
        bytesRead = errorCode.value();
        if (bytesRead == 0) {
            break;
        }
        totalBytesRead += bytesRead;
    }
    bytesRead = totalBytesRead;
    return io::Result<std::size_t>::Success(totalBytesRead);
}

// Read all bytes until EOF in this source, appending them to `buf`.
Result<std::size_t> Read::read_to_string(std::string &buf, size_t &bytesRead)
{
    std::vector<std::byte> tempBuf;
    auto result = read_to_end(tempBuf, bytesRead);
    if (result.isError()) {
        return io::Result<std::size_t>::Error(result.error());
    }
    buf.append(static_cast<const char *>(static_cast<const void *>(tempBuf.data())), tempBuf.size());
    return io::Result<std::size_t>::Success(bytesRead);
}

// Read the exact number of bytes required to fill `buf`.
Result<std::size_t> Read::read_exact(std::span<std::byte> buf)
{
    size_t bytesRead = 0;
    size_t totalBytesRead = 0;
    while (totalBytesRead < buf.size()) {
        auto result = read(buf.subspan(totalBytesRead));
        if (result.isError()) {
            return io::Result<std::size_t>::Error(result.error());
        }
        bytesRead = result.value();
        if (bytesRead == 0) {
            return io::Result<std::size_t>::Error(std::make_error_code(std::errc::io_error));
        }
        totalBytesRead += bytesRead;
    }
    return io::Result<std::size_t>::Success(totalBytesRead);
}
}