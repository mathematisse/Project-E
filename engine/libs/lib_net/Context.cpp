#include "lib_net/Context.hpp"

void net::Context::select(const std::vector<socket_t> &read_fds, const std::vector<socket_t> &write_fds, uint64_t timeout_ms)
{
    int maxFd = 0;
    struct timeval tv {};
    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000) * 1000;
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
    readyCount = ::select(maxFd + 1, &readFds, &writeFds, nullptr, &tv);
}
