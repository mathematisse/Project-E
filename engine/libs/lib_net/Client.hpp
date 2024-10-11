#include "lib_net/Packet.hpp"
#include "lib_net/Context.hpp"
#include "lib_net/TCPSocket.hpp"
#include "lib_net/UDPInfo.hpp"
#include <string>
#include <lib_net/net.hpp>
#include <optional>
#include <vector>

namespace net {

class Client {

    bool connect_tcp(const std::string &ip, uint16_t port) { return gateway.tcp_socket.connect(ip, port); }

    bool connect_udp(const std::string &ip, uint16_t port)
    {
        sockaddr_in addr {};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);

        gateway.udp_info.udp_address = addr;
        return true;
    }

    void send_tcp(Packet::MsgType type, const std::vector<std::uint8_t> &data)
    {
        gateway.send_tcp_queue.push_back(Packet::deserialize(type, data));
    }

    void send_udp(Packet::MsgType type, const std::vector<std::uint8_t> &data)
    {
        gateway.send_udp_queue.push_back(Packet::deserialize(type, data));
    }

    void update()
    {
        std::vector<socket_t> fds = {gateway.tcp_socket.getFD()};

        context.select(fds);

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

        for (Packet &packet : gateway.tcp_socket.readPackets()) {
            on_packet(packet);
        }
    }

    virtual void on_packet(const Packet &packet) = 0;

protected:
    socket_t udpFd;
    socket_t listenFd;
    Context context;
    Gateway gateway;
};

}