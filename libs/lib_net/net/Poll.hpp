#pragma once

#include <cstddef>
#include <optional>
#include <vector>

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
 * This class is a wrapper around the poll system call on Linux and the WSApoll system call on
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
    struct Fd {
        Socket::Type fd; /* file descriptor */
        short events; /* requested events */
        short revents; /* returned events */
        
        bool operator==(const Fd &other) const { return fd == other.fd; }
    };

    static constexpr int MAX_EVENTS = 64;

    Poll() = default;
    ~Poll() = default;

    Poll(const Poll &) = delete;
    Poll &operator=(const Poll &) = delete;

    Poll(Poll &&) = default;
    Poll &operator=(Poll &&) = default;

    [[nodiscard]]
    auto add(const TcpStream &stream) -> io::Result<result::Void>;
    [[nodiscard]]
    auto add(const UdpSocket &socket) -> io::Result<result::Void>;
    [[nodiscard]]
    auto add(const TcpListener &listener) -> io::Result<result::Void>;

    [[nodiscard]]
    auto add_write(const TcpStream &stream) -> io::Result<result::Void>;
    [[nodiscard]]
    auto add_write(const UdpSocket &socket) -> io::Result<result::Void>;

    [[nodiscard]]
    auto remove(const TcpStream &stream) -> io::Result<result::Void>;
    [[nodiscard]]
    auto remove(const UdpSocket &socket) -> io::Result<result::Void>;
    [[nodiscard]]
    auto remove(const TcpListener &listener) -> io::Result<result::Void>;

    auto remove_write(const TcpStream &stream) -> io::Result<result::Void>;
    auto remove_write(const UdpSocket &socket) -> io::Result<result::Void>;

    [[nodiscard]]
    auto wait(std::optional<std::size_t>) -> io::Result<std::vector<PollEvent>>;

private:
    std::vector<Poll::Fd> fds;
};

}