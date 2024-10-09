
#include "lib_net/TCPSocket.hpp"

namespace net {
bool TCPSocket::create()
{
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    return socket_fd != INVALID_SOCKET;
}

bool TCPSocket::bind(const std::string &ipAddress, uint16_t port) const
{
    sockaddr_in addr {};
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ipAddress.c_str());
    addr.sin_port = htons(port);

    return ::bind(socket_fd, (struct sockaddr *) &addr, sizeof(addr)) == 0;
}

bool TCPSocket::listen(int backlog) const { return ::listen(socket_fd, backlog) == 0; }

TCPSocket TCPSocket::accept() const
{
    TCPSocket clientSocket;
    sockaddr_in client_addr {};
    socklen_t client_len = sizeof(client_addr);
    clientSocket.socket_fd =
        ::accept(socket_fd, reinterpret_cast<struct sockaddr *>(&client_addr), &client_len);
    return clientSocket;
}

bool TCPSocket::connect(const std::string &ipAddress, uint16_t port) const
{
    sockaddr_in addr {};
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ipAddress.c_str());
    addr.sin_port = htons(port);

    return ::connect(socket_fd, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr)) == 0;
}

ssize_t TCPSocket::send(const std::vector<std::uint8_t> &data) const
{
    return ::send(socket_fd, reinterpret_cast<const char *>(data.data()), data.size(), 0);
}

ssize_t TCPSocket::recv(std::vector<std::uint8_t> &buffer, size_t size) const
{
    buffer.resize(size);
    return ::recv(socket_fd, reinterpret_cast<char *>(buffer.data()), size, 0);
}

ssize_t TCPSocket::recvToBuffer()
{
    std::vector<std::uint8_t> temp_buffer(MAX_BUFFER_SIZE); // Adjust size as needed
    ssize_t bytes_received = recv(temp_buffer, temp_buffer.size());
    if (bytes_received > 0) {
        buf_reader.append(temp_buffer);
    }
    return bytes_received;
}

std::optional<Packet> TCPSocket::readPacket() { return buf_reader.readPacket(); }

std::vector<Packet> TCPSocket::readPackets()
{
    std::vector<Packet> packets;

    while (auto packet = readPacket()) {
        packets.push_back(*packet);
    }
    return packets;
}

BufReader &TCPSocket::getBufReader() { return buf_reader; }

BufWriter &TCPSocket::getBufWriter() { return buf_writer; }

socket_t TCPSocket::getFD() const { return socket_fd; }

void TCPSocket::close()
{
    if (socket_fd != INVALID_SOCKET) {
        close_socket(socket_fd);
        socket_fd = INVALID_SOCKET;
    }
}

TCPSocket TCPSocket::accept(socket_t listenFD)
{
    TCPSocket clientSocket;
    sockaddr_in client_addr {};
    socklen_t client_len = sizeof(client_addr);
    clientSocket.socket_fd =
        ::accept(listenFD, reinterpret_cast<struct sockaddr *>(&client_addr), &client_len);
    return clientSocket;
}
}