#pragma once

#include "lib_net/net.hpp"
#include <cstdint>

namespace net {
struct Context {
    int max_sd;
    fd_set readfds;
    fd_set writefds;
    sigset_t mask;
    ssize_t ready_count;
    bool running;
};
};
