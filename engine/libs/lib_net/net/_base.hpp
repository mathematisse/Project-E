// Don't include this file in any header file (only in .cpp files)
#pragma once

#include <iostream>

#ifdef _WIN32
/* See http://stackoverflow.com/questions/12765743/getaddrinfo-on-win32 */
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501 /* Windows XP. */
#endif
#include <winsock2.h>
// #include <Ws2tcpip.h>
#include <ws2tcpip.h>

#include <mswsock.h>
#include <windows.h>
#pragma comment(lib, "Ws2_32.lib")
#else
/* Assume that any non-Windows platform uses POSIX-style sockets instead. */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <netdb.h> /* Needed for getaddrinfo() and freeaddrinfo() */
#include <unistd.h> /* Needed for close() */
#endif

namespace lnet {

constexpr size_t MAX_BUFFER_SIZE = 8192;
constexpr size_t MAX_LISTEN_BACKLOG = 5;
constexpr size_t MAX_LISTEN_QUEUE_SIZE = 100;

#ifdef _WIN32
using socket_t = SOCKET;
// #define SOCKET_ERROR (SOCKET_ERROR)
// #define INVALID_SOCKET (INVALID_SOCKET)
inline int close_socket(socket_t socket)
{
    int status = shutdown(socket, SD_BOTH);
    if (status == 0) {
        status = closesocket(socket);
    }
    return status;
}
#else
using socket_t = int;
#define SOCKET_ERROR (-1)
#define INVALID_SOCKET (-1)
inline int close_socket(socket_t socket)
{
    int status = shutdown(socket, SHUT_RDWR);
    if (status == 0) {
        status = close(socket);
    }
    return status;
}
#endif

class SocketInitializer {
public:
    SocketInitializer() noexcept
    {
#ifdef _WIN32
        WSADATA wsa_data;
        auto ret = WSAStartup(MAKEWORD(1, 1), &wsa_data);
#else
        auto ret = 0;
#endif
        if (ret != 0) {
            std::cerr << "Error initializing socket library: " << ret << std::endl;
        }
    }

    ~SocketInitializer() noexcept
    {
#ifdef _WIN32
        WSACleanup();
#else
#endif
    }
};

// Create a global instance of SocketInitializer to ensure initialization and cleanup
static const SocketInitializer socketInitializer;

}
