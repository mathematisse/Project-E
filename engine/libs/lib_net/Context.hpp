#pragma once

#include <map>

#include "lib_net/net.hpp"
#include "Gateway.hpp"

namespace net {
using client_id = std::uint32_t;

struct Context {
    fd_set readFds;
    fd_set writeFds;
    sigset_t mask;
    ssize_t readyCount;

    void select(std::vector<socket_t> &fds);
    [[nodiscard]] bool is_readable(socket_t fd) const;
    [[nodiscard]] bool is_writable(socket_t fd) const;
};
};
