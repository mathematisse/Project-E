#pragma once

#ifdef _WIN32
    #include <winsock2.h>
using SocketType = SOCKET;
    #pragma comment(lib, "Ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
using SocketType = int;
    #define INVALID_SOCKET (-1)
    #define SOCKET_ERROR (-1)
#endif

class Socket {
protected:
    SocketType sockfd;
    struct sockaddr_in address;

public:
    Socket();
    virtual ~Socket();
    bool bindSocket(int port);
    virtual bool closeSocket();
};
