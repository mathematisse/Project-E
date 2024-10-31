#include <algorithm>
#include <optional>

#include "lib_net/net/Poll.hpp"
#include "lib_net/net/_base.hpp"

namespace lnet::net {

auto Poll::add(const TcpStream &stream) -> io::Result<result::Void>
{
    for (auto &pfd : fds) {
        if (pfd.fd == stream._sock.sockfd) {
            pfd.events |= POLLIN;
            return io::Result<result::Void>::Success(result::Void {});
        }
    }
    Poll::Fd pfd {};
    pfd.fd = stream._sock.sockfd;
    pfd.events = POLLIN;
    fds.push_back(pfd);
    return io::Result<result::Void>::Success(result::Void {});
}

auto Poll::add(const UdpSocket &socket) -> io::Result<result::Void>
{
    for (auto &pfd : fds) {
        if (pfd.fd == socket._sock.sockfd) {
            pfd.events |= POLLIN;
            return io::Result<result::Void>::Success(result::Void {});
        }
    }
    Poll::Fd pfd {};
    pfd.fd = socket._sock.sockfd;
    pfd.events = POLLIN;
    fds.push_back(pfd);
    return io::Result<result::Void>::Success(result::Void {});
}

auto Poll::add(const TcpListener &listener) -> io::Result<result::Void>
{
    for (auto &pfd : fds) {
        if (pfd.fd == listener._sock.sockfd) {
            pfd.events |= POLLIN;
            return io::Result<result::Void>::Success(result::Void {});
        }
    }
    Poll::Fd pfd {};
    pfd.fd = listener._sock.sockfd;
    pfd.events = POLLIN;
    fds.push_back(pfd);
    return io::Result<result::Void>::Success(result::Void {});
}

auto Poll::add_write(const TcpStream &stream) -> io::Result<result::Void>
{
    for (auto &pfd : fds) {
        if (pfd.fd == stream._sock.sockfd) {
            pfd.events |= POLLOUT;
            return io::Result<result::Void>::Success(result::Void {});
        }
    }
    Poll::Fd pfd {};
    pfd.fd = stream._sock.sockfd;
    pfd.events = POLLOUT;
    fds.push_back(pfd);
    return io::Result<result::Void>::Success(result::Void {});
}

auto Poll::add_write(const UdpSocket &socket) -> io::Result<result::Void>
{
    for (auto &pfd : fds) {
        if (pfd.fd == socket._sock.sockfd) {
            pfd.events |= POLLOUT;
            return io::Result<result::Void>::Success(result::Void {});
        }
    }
    Poll::Fd pfd {};
    pfd.fd = socket._sock.sockfd;
    pfd.events = POLLOUT;
    fds.push_back(pfd);
    return io::Result<result::Void>::Success(result::Void {});
}

auto Poll::remove(const TcpStream &stream) -> io::Result<result::Void>
{
    auto it = std::remove_if(fds.begin(), fds.end(), [&](const Poll::Fd &pfd) {
        return pfd.fd == stream._sock.sockfd;
    });
    fds.erase(it, fds.end());
    return io::Result<result::Void>::Success(result::Void {});
}

auto Poll::remove(const UdpSocket &socket) -> io::Result<result::Void>
{
    auto it = std::remove_if(fds.begin(), fds.end(), [&](const Poll::Fd &pfd) {
        return pfd.fd == socket._sock.sockfd;
    });
    fds.erase(it, fds.end());
    return io::Result<result::Void>::Success(result::Void {});
}

auto Poll::remove(const TcpListener &listener) -> io::Result<result::Void>
{
    auto it = std::remove_if(fds.begin(), fds.end(), [&](const Poll::Fd &pfd) {
        return pfd.fd == listener._sock.sockfd;
    });
    fds.erase(it, fds.end());
    return io::Result<result::Void>::Success(result::Void {});
}

auto Poll::wait(std::optional<std::size_t> timeout) -> io::Result<std::vector<PollEvent>>
{
    int poll_timeout = timeout.has_value() ? static_cast<int>(*timeout) : -1;
#ifdef __linux__
    int nfds = ::poll(reinterpret_cast<pollfd *>(fds.data()), fds.size(), poll_timeout);
#else
    int nfds = ::WSAPoll(reinterpret_cast<WSAPOLLFD *>(fds.data()), fds.size(), poll_timeout);
#endif

    if (nfds == -1) {
        return io::Result<std::vector<PollEvent>>::Error(
            std::error_code(errno, std::system_category())
        );
    }

    std::vector<PollEvent> poll_events;
    for (const auto &pfd : fds) {
        if (pfd.revents & POLLIN) {
            poll_events.emplace_back(PollEvent::Type::Read, pfd.fd);
        }
        if (pfd.revents & POLLOUT) {
            poll_events.emplace_back(PollEvent::Type::Write, pfd.fd);
        }
        if (pfd.revents & POLLERR) {
            poll_events.emplace_back(PollEvent::Type::Error, pfd.fd);
        }
    }
    return io::Result<std::vector<PollEvent>>::Success(poll_events);
}

auto Poll::remove_write(const TcpStream &stream) -> io::Result<result::Void>
{
    for (auto &pfd : fds) {
        if (pfd.fd == stream._sock.sockfd) {
            pfd.events &= ~POLLOUT;
            if (pfd.events == 0) {
                fds.erase(std::remove(fds.begin(), fds.end(), pfd), fds.end());
            }
            return io::Result<result::Void>::Success(result::Void {});
        }
    }
    return io::Result<result::Void>::Success(result::Void {});
}

auto Poll::remove_write(const UdpSocket &socket) -> io::Result<result::Void>
{
    for (auto &pfd : fds) {
        if (pfd.fd == socket._sock.sockfd) {
            pfd.events &= ~POLLOUT;
            if (pfd.events == 0) {
                fds.erase(std::remove(fds.begin(), fds.end(), pfd), fds.end());
            }
            return io::Result<result::Void>::Success(result::Void {});
        }
    }
    return io::Result<result::Void>::Success(result::Void {});
}

}
