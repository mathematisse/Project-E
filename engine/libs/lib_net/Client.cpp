#include "Client.hpp"
#include <optional>

bool net::Client::connect_tcp(const std::string &ip, uint16_t port)
{
    return gateway.tcp_socket.create() && gateway.tcp_socket.connect(ip, port);
}

bool net::Client::establish_udp_connection(const std::string &ip, uint16_t port)
{
    sockaddr_in addr {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);

    udpFd = socket(AF_INET, SOCK_DGRAM, 0);
    if (udpFd == INVALID_SOCKET) {
        return false;
    }
    gateway.udp_info.udp_address = addr;
    return true;
}

void net::Client::send_tcp(Packet::MsgType type, const std::vector<std::uint8_t> &data)
{
    gateway.send_tcp_queue.push_back(Packet::deserialize(type, data));
}

void net::Client::send_udp(Packet::MsgType type, const std::vector<std::uint8_t> &data)
{
    gateway.send_udp_queue.push_back(Packet::deserialize(type, data));
}

void net::Client::update()
{
    std::vector<socket_t> read_fds = {udpFd};
    std::vector<socket_t> write_fds = {};

    if (gateway.tcp_socket.getFD() != INVALID_SOCKET) {
        read_fds.push_back(gateway.tcp_socket.getFD());
    }
    if (gateway.tcp_socket.getFD() != INVALID_SOCKET && !gateway.send_tcp_queue.empty()) {
        write_fds.push_back(gateway.tcp_socket.getFD());
    }
    if (!gateway.send_udp_queue.empty()) {
        write_fds.push_back(udpFd);
    }

    context.select(read_fds, write_fds, std::nullopt);

    // TCP
    if (context.is_readable(gateway.tcp_socket.getFD())) {
        context.readyCount--;
        if (gateway.tcp_socket.recvToBuffer() == 0) {
            gateway.tcp_socket.close();
        }
    }
    if (context.is_writable(gateway.tcp_socket.getFD())) {
        context.readyCount--;
        gateway.tcp_socket.getBufWriter().appendPackets(gateway.send_tcp_queue);
        gateway.send_tcp_queue.clear();
        ssize_t byte_sent = gateway.tcp_socket.send(gateway.tcp_socket.getBufWriter().getBuffer());
        if (byte_sent < 0) {
            gateway.tcp_socket.close();
        } else {
            gateway.tcp_socket.getBufWriter().consume(byte_sent);
        }
    }

    if (context.readyCount < 0) {
        return;
    }

    // UDP
    if (context.is_readable(udpFd)) {
        context.readyCount--;
        sockaddr_in addr {};
        std::vector<std::uint8_t> buffer(MAX_BUFFER_SIZE);
        socklen_t addr_len = sizeof(addr);
        ssize_t bytes_received = recvfrom(
            udpFd, reinterpret_cast<char *>(buffer.data()), buffer.size(), 0,
            reinterpret_cast<struct sockaddr *>(&addr), &addr_len
        );

        if (bytes_received > 0) {
            buffer.resize(bytes_received);
            gateway.udp_info.buf_reader.append(buffer);
        }
    }

    if (context.is_writable(udpFd)) {
        context.readyCount--;
        gateway.udp_info.buf_writer.appendPackets(gateway.send_udp_queue);
        gateway.send_udp_queue.clear();
        ssize_t byte_sent = sendto(
            udpFd, reinterpret_cast<const char *>(gateway.udp_info.buf_writer.getBuffer().data()),
            gateway.udp_info.buf_writer.getBuffer().size(), 0,
            reinterpret_cast<struct sockaddr *>(&gateway.udp_info.udp_address),
            sizeof(gateway.udp_info.udp_address)
        );

        if (byte_sent < 0) {
            gateway.tcp_socket.close();
        } else {
            gateway.udp_info.buf_writer.consume(byte_sent);
        }
    }

    for (Packet &packet : gateway.tcp_socket.readPackets()) {
        on_packet(packet);
    }

    for (Packet &packet : gateway.udp_info.readPackets()) {
        on_packet(packet);
    }
}
