#include "lib_net/Context.hpp"

void net::Context::select(std::vector<socket_t> &fds)
{
    int maxFd = 0;
    FD_ZERO(&readFds);
    FD_ZERO(&writeFds);

    for (auto &fd : fds) {
        FD_SET(fd, &readFds);
        FD_SET(fd, &writeFds);
        if (fd > maxFd) {
            maxFd = fd;
        }
    }
    readyCount = ::select(maxFd + 1, &readFds, &writeFds, nullptr, nullptr);
}

bool net::Context::is_readable(socket_t fd) const { return FD_ISSET(fd, &readFds); }

bool net::Context::is_writable(socket_t fd) const { return FD_ISSET(fd, &writeFds); }
