
#ifdef __linux__
#include <cstddef>
#include <optional>

#include <sys/epoll.h>
#include <unistd.h>

#include "Poll.hpp"

namespace net::net {

auto Poll::add_read(const TcpStream &stream) -> io::Result<result::Void>
{
    epoll_event event {};
    event.events = EPOLLIN;
    event.data.fd = stream._sock.sockfd;
    if (epoll_ctl(_impl.epoll_fd, EPOLL_CTL_ADD, stream._sock.sockfd, &event) == -1) {
        return io::Result<result::Void>::Error(errno);
    }
    return io::Result<result::Void>::Success(result::Void {});
}

auto Poll::add_read(const UdpSocket &socket) -> io::Result<result::Void>
{
    epoll_event event {};
    event.events = EPOLLIN;
    event.data.fd = socket._sock.sockfd;
    if (epoll_ctl(_impl.epoll_fd, EPOLL_CTL_ADD, socket._sock.sockfd, &event) == -1) {
        return io::Result<result::Void>::Error(errno);
    }
    return io::Result<result::Void>::Success(result::Void {});
}

auto Poll::add_read(const TcpListener &listener) -> io::Result<result::Void>
{
    epoll_event event {};
    event.events = EPOLLIN;
    event.data.fd = listener._sock.sockfd;
    if (epoll_ctl(_impl.epoll_fd, EPOLL_CTL_ADD, listener._sock.sockfd, &event) == -1) {
        return io::Result<result::Void>::Error(errno);
    }
    return io::Result<result::Void>::Success(result::Void {});
}

auto Poll::add_write(const TcpStream &stream) -> io::Result<result::Void>
{
    epoll_event event {};
    event.events = EPOLLOUT;
    event.data.fd = stream._sock.sockfd;
    if (epoll_ctl(_impl.epoll_fd, EPOLL_CTL_ADD, stream._sock.sockfd, &event) == -1) {
        return io::Result<result::Void>::Error(errno);
    }
    return io::Result<result::Void>::Success(result::Void {});
}

auto Poll::add_write(const UdpSocket &socket) -> io::Result<result::Void>
{
    epoll_event event {};
    event.events = EPOLLOUT;
    event.data.fd = socket._sock.sockfd;
    if (epoll_ctl(_impl.epoll_fd, EPOLL_CTL_ADD, socket._sock.sockfd, &event) == -1) {
        return io::Result<result::Void>::Error(errno);
    }
    return io::Result<result::Void>::Success(result::Void {});
}

auto Poll::remove_read(const TcpStream &stream) -> io::Result<result::Void>
{
    if (epoll_ctl(_impl.epoll_fd, EPOLL_CTL_DEL, stream._sock.sockfd, nullptr) == -1) {
        return io::Result<result::Void>::Error(errno);
    }
    return io::Result<result::Void>::Success(result::Void {});
}

auto Poll::remove_read(const UdpSocket &socket) -> io::Result<result::Void>
{
    if (epoll_ctl(_impl.epoll_fd, EPOLL_CTL_DEL, socket._sock.sockfd, nullptr) == -1) {
        return io::Result<result::Void>::Error(errno);
    }
    return io::Result<result::Void>::Success(result::Void {});
}

auto Poll::remove_read(const TcpListener &listener) -> io::Result<result::Void>
{
    if (epoll_ctl(_impl.epoll_fd, EPOLL_CTL_DEL, listener._sock.sockfd, nullptr) == -1) {
        return io::Result<result::Void>::Error(errno);
    }
    return io::Result<result::Void>::Success(result::Void {});
}

auto Poll::remove_write(const TcpStream &stream) -> io::Result<result::Void>
{
    if (epoll_ctl(_impl.epoll_fd, EPOLL_CTL_DEL, stream._sock.sockfd, nullptr) == -1) {
        return io::Result<result::Void>::Error(errno);
    }
    return io::Result<result::Void>::Success(result::Void {});
}

auto Poll::remove_write(const UdpSocket &socket) -> io::Result<result::Void>
{
    if (epoll_ctl(_impl.epoll_fd, EPOLL_CTL_DEL, socket._sock.sockfd, nullptr) == -1) {
        return io::Result<result::Void>::Error(errno);
    }
    return io::Result<result::Void>::Success(result::Void {});
}

auto Poll::wait(std::optional<std::size_t> timeout) -> io::Result<std::vector<PollEvent>>
{
    std::vector<epoll_event> events(Poll::MAX_EVENTS);
    int nfds = 0;
    if (timeout.has_value()) {
        nfds = epoll_wait(
            _impl.epoll_fd, events.data(), static_cast<int>(events.size()), timeout.value()
        );
    } else {
        nfds = epoll_wait(_impl.epoll_fd, events.data(), static_cast<int>(events.size()), -1);
    }
    if (nfds == -1) {
        return io::Result<std::vector<PollEvent>>::Error(errno);
    }

    std::vector<PollEvent> poll_events;
    for (int i = 0; i < nfds; i++) {
        if ((events[i].events & EPOLLIN) != 0U) {
            poll_events.emplace_back(PollEvent::Type::Read, events[i].data.fd);
        }
        if ((events[i].events & EPOLLOUT) != 0U) {
            poll_events.emplace_back(PollEvent::Type::Write, events[i].data.fd);
        }
        if ((events[i].events & EPOLLERR) != 0U) {
            poll_events.emplace_back(PollEvent::Type::Error, events[i].data.fd);
        }
    }
    return io::Result<std::vector<PollEvent>>::Success(poll_events);
}

}
#endif
