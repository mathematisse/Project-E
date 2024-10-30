#pragma once

#include <map>

#include "lib_net/net.hpp"
#include "lib_net/uuid/Uuid.hpp"
#include "Gateway.hpp"

namespace net {
using client_id = lnet::uuid::Uuid;

struct Context {
    fd_set readFds;
    fd_set writeFds;
    ssize_t readyCount;

    void select(const std::vector<socket_t> &read_fds, const std::vector<socket_t> &write_fds);
    [[nodiscard]] inline bool is_readable(socket_t fd) const
    {
        return (fd != INVALID_SOCKET && FD_ISSET(fd, &readFds));
    }
    [[nodiscard]] inline bool is_writable(socket_t fd) const
    {
        return (fd != INVALID_SOCKET && FD_ISSET(fd, &writeFds));
    }
};
};
