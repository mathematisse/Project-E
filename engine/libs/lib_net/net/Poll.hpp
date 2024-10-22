#pragma once

#include <cstddef>
#include <optional>
#include <vector>

#include "lib_net/net/_base.hpp"
#include "lib_net/io/Result.hpp"
#include "lib_net/net/TcpStream.hpp"
#include "lib_net/net/UdpSocket.hpp"
#include "lib_net/net/TcpListener.hpp"

namespace lnet::net {

class PollEvent {
public:
    enum class Type {
        Read,
        Write,
        Error
    };

    PollEvent(Type type, int fd):
        type(type),
        fd(fd)
    {
    }

    Type type;
    int fd;
};

/**
 * @brief Poll for I/O events on sockets
 *
 * This class is a wrapper around the epoll system call on Linux and the IOCP system call on
 * Windows. It allows for polling for read and write events on sockets.
 *
 * # Example
 *
 * ```cpp
 * auto poll = Poll();
 * auto stream = TcpStream::connect(SocketAddr((IpAddr(Ipv4Addr({127, 0, 0, 1})), 8080)));
 * poll.add_read(stream);
 * auto events = poll.wait(std::nullopt);
 * for (const auto &event : events.value()) {
 *     switch (event.type) {
 *         case PollEvent::Type::Read:
 *             // handle read event
 *             break;
 *         case PollEvent::Type::Write:
 *             // handle write event
 *             break;
 *         case PollEvent::Type::Error:
 *             // handle error event
 *             break;
 *     }
 * }
 * ```
 */
class Poll {
public:
    static constexpr int MAX_EVENTS = 64;

    Poll() = default;
    ~Poll() = default;

    Poll(const Poll &) = delete;
    Poll &operator=(const Poll &) = delete;

    Poll(Poll &&) = default;
    Poll &operator=(Poll &&) = default;

    [[nodiscard]]
    auto add_read(const TcpStream &stream) -> io::Result<result::Void>;
    [[nodiscard]]
    auto add_read(const UdpSocket &socket) -> io::Result<result::Void>;
    [[nodiscard]]
    auto add_read(const TcpListener &listener) -> io::Result<result::Void>;

    [[nodiscard]]
    auto add_write(const TcpStream &stream) -> io::Result<result::Void>;
    [[nodiscard]]
    auto add_write(const UdpSocket &socket) -> io::Result<result::Void>;

    [[nodiscard]]
    auto remove_read(const TcpStream &stream) -> io::Result<result::Void>;
    [[nodiscard]]
    auto remove_read(const UdpSocket &socket) -> io::Result<result::Void>;
    [[nodiscard]]
    auto remove_read(const TcpListener &listener) -> io::Result<result::Void>;

    [[nodiscard]]
    auto remove_write(const TcpStream &stream) -> io::Result<result::Void>;
    [[nodiscard]]
    auto remove_write(const UdpSocket &socket) -> io::Result<result::Void>;

    [[nodiscard]]
    auto wait(std::optional<std::size_t>) -> io::Result<std::vector<PollEvent>>;

private:
#ifdef __linux__
    struct Impl {
        int epoll_fd;

        Impl():
            epoll_fd(epoll_create1(0))
        {
            if (epoll_fd == -1) {
                throw std::system_error(errno, std::generic_category(), "epoll_create1 failed");
            }
        }

        ~Impl() { close(epoll_fd); }
    };
#elif _WIN32
    struct Impl {
        HANDLE iocp;

        Impl()
        {
            iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
            if (iocp == nullptr) {
                throw std::system_error(
                    GetLastError(), std::system_category(), "CreateIoCompletionPort failed"
                );
            }
        }

        ~Impl() { CloseHandle(iocp); }
    };
#else
#error "Unsupported platform"
#endif
    Impl _impl;
};

}