
#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "Socket.hpp"

class TCPSocket : public Socket {
public:
    bool listenSocket(int backlog = 5);
    bool acceptSocket(TCPSocket &newSocket);
    bool connectSocket(const std::string &host, int port);
    ssize_t sendData(const std::vector<uint8_t> &data);
    ssize_t receiveData(std::vector<uint8_t> &data, size_t maxSize = 1024);
};
