#include "lib_net/Context.hpp"

void net::Context::select(const std::vector<socket_t> &read_fds, const std::vector<socket_t> &write_fds)
{
    int maxFd = 0;
    FD_ZERO(&readFds);
    FD_ZERO(&writeFds);

    for (const auto fd : read_fds) {
        FD_SET(fd, &readFds);
        maxFd = std::max(maxFd, fd);
    }
    for (const auto fd : write_fds) {
        FD_SET(fd, &writeFds);
        maxFd = std::max(maxFd, fd);
    }
    readyCount = ::select(maxFd + 1, &readFds, &writeFds, nullptr, nullptr);
}
