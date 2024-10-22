#ifdef _WIN32
#include <system_error>

#include "lib_net/net/_base.hpp"
#include "lib_net/net/Poll.hpp"

namespace lnet::net {

auto Poll::add_read(const TcpStream &stream) -> io::Result<result::Void>
{
    if (CreateIoCompletionPort(
            (HANDLE) stream._sock.sockfd, _impl.iocp, (ULONG_PTR) stream._sock.sockfd, 0
        ) == nullptr) {
        return io::Result<result::Void>::Error(GetLastError());
    }
    return io::Result<result::Void>::Success(result::Void {});
}

auto Poll::add_read(const UdpSocket &socket) -> io::Result<result::Void>
{
    if (CreateIoCompletionPort(
            (HANDLE) socket._sock.sockfd, _impl.iocp, (ULONG_PTR) socket._sock.sockfd, 0
        ) == nullptr) {
        return io::Result<result::Void>::Error(GetLastError());
    }
    return io::Result<result::Void>::Success(result::Void {});
}

auto Poll::add_read(const TcpListener &listener) -> io::Result<result::Void>
{
    if (CreateIoCompletionPort(
            (HANDLE) listener._sock.sockfd, _impl.iocp, (ULONG_PTR) listener._sock.sockfd, 0
        ) == nullptr) {
        return io::Result<result::Void>::Error(GetLastError());
    }
    return io::Result<result::Void>::Success(result::Void {});
}

auto Poll::add_write(const TcpStream &stream) -> io::Result<result::Void>
{
    if (CreateIoCompletionPort(
            (HANDLE) stream._sock.sockfd, _impl.iocp, (ULONG_PTR) stream._sock.sockfd, 0
        ) == nullptr) {
        return io::Result<result::Void>::Error(GetLastError());
    }
    return io::Result<result::Void>::Success(result::Void {});
}

auto Poll::add_write(const UdpSocket &socket) -> io::Result<result::Void>
{
    if (CreateIoCompletionPort(
            (HANDLE) socket._sock.sockfd, _impl.iocp, (ULONG_PTR) socket._sock.sockfd, 0
        ) == nullptr) {
        return io::Result<result::Void>::Error(GetLastError());
    }
    return io::Result<result::Void>::Success(result::Void {});
}

auto Poll::remove_read(const TcpStream &stream) -> io::Result<result::Void>
{
    // No need to remove from IOCP, just stop issuing read operations
    return io::Result<result::Void>::Success(result::Void {});
}

auto Poll::remove_read(const UdpSocket &socket) -> io::Result<result::Void>
{
    // No need to remove from IOCP, just stop issuing read operations
    return io::Result<result::Void>::Success(result::Void {});
}

auto Poll::remove_read(const TcpListener &listener) -> io::Result<result::Void>
{
    // No need to remove from IOCP, just stop issuing read operations
    return io::Result<result::Void>::Success(result::Void {});
}

auto Poll::remove_write(const TcpStream &stream) -> io::Result<result::Void>
{
    // No need to remove from IOCP, just stop issuing write operations
    return io::Result<result::Void>::Success(result::Void {});
}

auto Poll::remove_write(const UdpSocket &socket) -> io::Result<result::Void>
{
    // No need to remove from IOCP, just stop issuing write operations
    return io::Result<result::Void>::Success(result::Void {});
}

auto Poll::wait(std::optional<std::size_t> timeout) -> io::Result<std::vector<PollEvent>>
{
    std::vector<PollEvent> poll_events;
    DWORD bytes_transferred;
    ULONG_PTR completion_key;
    OVERLAPPED *overlapped;

    DWORD wait_time = timeout.has_value() ? static_cast<DWORD>(*timeout) : INFINITE;

    while (GetQueuedCompletionStatus(
        _impl.iocp, &bytes_transferred, &completion_key, &overlapped, wait_time
    )) {
        int fd = static_cast<int>(completion_key);
        poll_events.emplace_back(PollEvent::Type::Read, fd);
    }

    if (GetLastError() != ERROR_SUCCESS) {
        return io::Result<std::vector<PollEvent>>::Error(GetLastError());
    }

    return io::Result<std::vector<PollEvent>>::Success(poll_events);
}

}
#endif