#pragma once

#include <vector>
#include <string>

#include "Socket.hpp"

class UDPSocket : public Socket {
public:
    bool sendTo(const std::string &host, int port, const std::vector<uint8_t> &data);
    ssize_t receiveFrom(std::vector<uint8_t> &data, size_t maxSize = 1024);
};
