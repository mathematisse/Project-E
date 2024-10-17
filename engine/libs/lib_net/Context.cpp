#include "lib_net/Context.hpp"
#include <optional>

void net::Context::select(
    const std::vector<socket_t> &read_fds, const std::vector<socket_t> &write_fds,
    std::optional<uint64_t> timeout_ms
)
{
    int maxFd = 0;

    if (timeout_ms.has_value()) {
        tv.tv_sec = timeout_ms.value() / 1000;
        tv.tv_usec = (timeout_ms.value() % 1000) * 1000;
    }
    FD_ZERO(&readFds);
    FD_ZERO(&writeFds);

    for (const auto fd : read_fds) {
        FD_SET(fd, &readFds);
        if (fd > maxFd) {
            maxFd = fd;
        }
    }
    for (const auto fd : write_fds) {
        FD_SET(fd, &writeFds);
        if (fd > maxFd) {
            maxFd = fd;
        }
    }
<<<<<<< HEAD
    if (timeout_ms.has_value()) {
        readyCount = ::select(maxFd + 1, &readFds, &writeFds, nullptr, &tv);
    } else {
        readyCount = ::select(maxFd + 1, &readFds, &writeFds, nullptr, nullptr);
    }
=======

    // set timeout to 0 to return make select non-blocking (not a good idea)
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;

    readyCount = ::select(maxFd + 1, &readFds, &writeFds, nullptr, &tv);
>>>>>>> eaeff115c56901c94ba580ebe70048db6fbf31ad
}
