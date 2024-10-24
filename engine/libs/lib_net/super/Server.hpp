
#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <unordered_map>
#include <vector>

#include "lib_net/Packet.hpp"
#include "lib_net/net/SocketAddr.hpp"
#include "lib_net/net/TcpStream.hpp"
#include "lib_net/super/BaseServer.hpp"
#include "lib_net/uuid/Uuid.hpp"
#include "lib_net/uuid/UuidGenerator.hpp"

namespace net {

// This server class is a base class for a server that handles clients with both TCP and UDP
// connections. UDP Connection are linked to TCP connections by a UUID.
class Server : public lnet::utils::BaseServer {
public:
    virtual void on_tcp_connect(lnet::uuid::Uuid client_uuid) = 0;
    virtual void on_tcp_disconnect(lnet::uuid::Uuid client_uuid) = 0;

    virtual void on_udp_connect(lnet::uuid::Uuid client_uuid) = 0;
    virtual void on_packet(const Packet &packet, lnet::uuid::Uuid client_uuid) = 0;

    // using the same port for both TCP and UDP (for simplicity)
    auto host(std::uint16_t port
    ) -> lnet::result::Result<lnet::result::Void, lnet::utils::BaseServerError>
    {
        if (auto res = host_tcp(port); !res) {
            return res;
        }
        return host_udp(port);
    }

    auto start() -> decltype(start_context()) { return start_context(); }
    auto stop() -> void { return stop_context(); }

    void send_tcp(
        lnet::uuid::Uuid client_uuid, Packet::MsgType type, const std::vector<std::uint8_t> &data
    )
    {
        auto matchingClient = _clients.find(client_uuid);
        Packet packet = Packet::deserialize(type, data);
        if (matchingClient != _clients.end()) {
            lnet::utils::BaseServer::send_tcp(
                matchingClient->second.tcp_connection_id, packet.serialize()
            );
        }
    }

    void send_udp(
        lnet::uuid::Uuid client_uuid, Packet::MsgType type, const std::vector<std::uint8_t> &data
    )
    {
        auto matchingClient = _clients.find(client_uuid);
        Packet packet = Packet::deserialize(type, data);
        if (matchingClient != _clients.end()) {
            lnet::utils::BaseServer::send_udp(matchingClient->second.udp_addr, packet.serialize());
        }
    }

    void send_tcp_all(Packet::MsgType type, const std::vector<std::uint8_t> &data)
    {
        Packet packet = Packet::deserialize(type, data);
        for (const auto &[client_uuid, client] : _clients) {
            lnet::utils::BaseServer::send_tcp(client.tcp_connection_id, packet.serialize());
        }
    }

    void send_udp_all(Packet::MsgType type, const std::vector<std::uint8_t> &data)
    {
        Packet packet = Packet::deserialize(type, data);
        for (const auto &[client_uuid, client] : _clients) {
            lnet::utils::BaseServer::send_udp(client.udp_addr, packet.serialize());
        }
    }

    /*
        BaseServer.hpp theses are transformed to higher order events common for TCP & UDP
        To be able to give the client_uuid to the user instead of the tcp_connection_id
    */
    void on_tcp_connection(lnet::uuid::Uuid id) override
    {
        // get client UUID from the connection ID
        auto matchingClient = _clients.find(id);
        if (matchingClient != _clients.end()) {
            on_tcp_connect(matchingClient->first);
        } else {
            // should not happen, could happen if the client is disconnected before the connection
        }
    }
    void on_tcp_disconnection(lnet::uuid::Uuid id) override
    {
        // get client UUID from the connection ID
        auto matchingClient = _clients.find(id);
        if (matchingClient != _clients.end()) {
            on_tcp_disconnect(matchingClient->first);
        } else {
            // should not happen, could happen if the client is disconnected before the connection
        }
    }

    void on_udp_data(const lnet::net::SocketAddr &addr, const std::vector<std::byte> &data)
    {
        // get the matching client UUID from the UDP address
        // if the client is not found
        // if this is a new
    }

    void on_tcp_data(lnet::uuid::Uuid id) { }

private:
    struct Client {
        lnet::uuid::Uuid tcp_connection_id;
        lnet::net::SocketAddr udp_addr;
    };

    lnet::uuid::UuidGenerator<> client_uuid_generator;
    std::unordered_map<lnet::uuid::Uuid, Client> _clients;
};

} // namespace net
