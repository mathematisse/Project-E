
#pragma once

#include <algorithm>
#include <cstdint>
#include <map>
#include <optional>
#include <random>
#include <span>
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
// All functions should be called from the same thread (the thread that calls update)
class Server : private lnet::utils::BaseServer {
public:
    struct Client {
        lnet::uuid::Uuid tcp_connection_id;
        lnet::net::SocketAddr udp_addr;
        std::uint64_t generated_number;
    };

    Server() = default;
    ~Server() override = default;

    virtual void on_tcp_connect(lnet::uuid::Uuid client_uuid) = 0;
    virtual void on_tcp_disconnect(lnet::uuid::Uuid client_uuid) = 0;

    virtual void on_udp_connect(lnet::uuid::Uuid client_uuid) = 0;
    virtual void on_packet(const Packet &packet, lnet::uuid::Uuid client_uuid) = 0;

protected:
    // modifies the number given for security reasons (to prevent replay attacks)
    // The function should be complex enough to prevent the client from guessing the function
    // The function should not give the same result for different numbers
    virtual auto transformNumberFunction(std::uint64_t number) -> std::uint64_t
    {
        // example function (should be overriden)
        constexpr std::uint64_t SECRET_NUMBER = 0x1234567890ABCDEF;
        return number ^ SECRET_NUMBER;
    }

public:
    // using the same port for both TCP and UDP (for simplicity)
    auto host(std::uint16_t port
    ) -> lnet::result::Result<lnet::result::Void, lnet::utils::BaseServerError>
    {
        if (auto res = host_tcp(port); !res) {
            return res;
        }
        return host_udp(port);
    }

    auto connect_tcp(const std::string &ip, std::uint16_t port)
        -> lnet::result::Result<lnet::result::Void, lnet::utils::BaseServerError>
    {
        return lnet::utils::BaseServer::connect_tcp(ip, port);
    }

    auto connect_udp(const std::string &ip, std::uint16_t port)
        -> lnet::result::Result<lnet::result::Void, lnet::utils::BaseServerError>
    {
        return lnet::utils::BaseServer::connect_udp(ip, port);
    }

    auto start() -> decltype(start_context()) { return start_context(); }
    auto stop() -> void { return stop_context(); }
    auto update() -> void { return handle_events(); }

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
        From BaseServer.hpp theses are transformed to higher order events common for TCP & UDP
        To be able to give the client_uuid to the user instead of the tcp_connection_id
    */
    void on_tcp_connection(lnet::uuid::Uuid tcp_connection_id) override
    {
        auto client_uuid = client_uuid_generator.new_uuid();
        _clients.insert({client_uuid, Client {tcp_connection_id}});
        on_tcp_connect(client_uuid);
        ask_udp_connection(client_uuid);
    }

    void on_tcp_disconnection(lnet::uuid::Uuid tcp_connection_id) override
    {
        // get client UUID from the connection ID
        auto matchingClient =
            std::find_if(_clients.begin(), _clients.end(), [tcp_connection_id](const auto &client) {
                return client.second.tcp_connection_id == tcp_connection_id;
            });
        if (matchingClient != _clients.end()) {
            on_tcp_disconnect(matchingClient->first);
            // remove the UDP connection if it exists
            if (auto udp_it = _udp_connection_cache.find(matchingClient->second.udp_addr);
                udp_it != _udp_connection_cache.end()) {
                _udp_connection_cache.erase(udp_it);
            }
            // remove the client from the list
            _clients.erase(matchingClient);
        } else {
            // should not happen, could happen if the client is disconnected before the connection
        }
    }

    /*
        on_tcp and on_udp have reserved space in the packets for lower level events that handle
        the udp connection request and the udp connection response
    */
    void on_udp_data(const lnet::net::SocketAddr &addr, const std::vector<std::byte> &data) override
    {
        // if the client is already connected with UDP on_packet will be called else we call
        // handle_udp_connection_request
        if (auto matchingClient = _udp_connection_cache.find(addr);
            matchingClient != _udp_connection_cache.end()) {
            auto packet = Packet::deserialize(data);
            if (packet.has_value()) {
                on_packet(packet.value(), matchingClient->second);
            }
        } else {
            handle_udp_connection_request(data, addr);
        }
    }

    void on_tcp_data(
        lnet::uuid::Uuid tcp_connection_id,
        lnet::io::Mutex<lnet::io::BufReader<lnet::net::TcpStream>> &stream
    ) override
    {
        auto matchingClient =
            std::find_if(_clients.begin(), _clients.end(), [tcp_connection_id](const auto &client) {
                return client.second.tcp_connection_id == tcp_connection_id;
            });
        if (matchingClient != _clients.end()) {
            std::optional<Packet> packet;
            std::span<std::byte> buffer;
            stream.with_lock([&buffer, &packet](auto &reader) {
                buffer = reader.buffer();
                packet = Packet::deserialize(buffer);
            });

            while (packet.has_value()) {
                on_packet(packet.value(), matchingClient->first);
                stream.with_lock([&buffer, &packet](auto &reader) {
                    reader.consume(packet->size());
                    buffer = reader.buffer();
                });
                packet = Packet::deserialize(buffer);
            }
        }
    }

    bool has_udp_connection(lnet::uuid::Uuid client_uuid) const
    {
        auto matchingClient = _clients.find(client_uuid);
        if (matchingClient != _clients.end()) {
            return _udp_connection_cache.find(matchingClient->second.udp_addr) !=
                _udp_connection_cache.end();
        }
        return false;
    }

private:
    struct UdpConnectionPacketInitialisation {
        lnet::uuid::Uuid client_uuid;
        std::uint64_t generated_number;
    };

    struct UdpConnectionPacketConfirmation {
        lnet::uuid::Uuid client_uuid;
        std::uint64_t transformed_number;
    };

    // ask the client to send the UDP connection request (tell the client that the server is ready
    // to take UDP connection requests)
    // give the client UUID and some random number to the user, and expect the client to send the
    // UUID back as a tranformed number
    void ask_udp_connection(lnet::uuid::Uuid client_uuid)
    {
        auto matchingClient = _clients.find(client_uuid);
        if (matchingClient != _clients.end()) {
            auto number = static_cast<std::uint64_t>(_number_generator());
            matchingClient->second.generated_number = number;
            Packet packet = Packet::deserialize(
                Packet::SystemTypes::ASKUDP_NUMBER,
                Packet::serializeStruct(UdpConnectionPacketInitialisation {client_uuid, number})
            );
            lnet::utils::BaseServer::send_tcp(
                matchingClient->second.tcp_connection_id, packet.serialize()
            );
        }
    }

    // First connection
    void handle_udp_connection_request(
        const std::vector<std::byte> &data, const lnet::net::SocketAddr &addr
    )
    {
        if (auto packet = Packet::deserialize(data); packet.has_value()) {
            if (packet->header.type == Packet::SystemTypes::ASKUDP_RESPONSE) {
                if (auto udp_data =
                        Packet::deserializeStruct<UdpConnectionPacketConfirmation>(packet->data);
                    udp_data.has_value()) {
                    auto matchingClient = _clients.find(udp_data->client_uuid);
                    // check if the client is indeed the one that sent the request
                    if (matchingClient != _clients.end() &&
                        transformNumberFunction(matchingClient->second.generated_number) ==
                            udp_data->transformed_number) {
                        // the client is verified
                        matchingClient->second.udp_addr = addr;
                        _udp_connection_cache.insert({addr, udp_data->client_uuid});
                        on_udp_connect(udp_data->client_uuid);
                    }
                }
            }
        }
        // else ignore the packet
    }

    std::mt19937_64 _number_generator = std::mt19937_64(std::random_device()());
    lnet::uuid::UuidGenerator<> client_uuid_generator =
        lnet::uuid::UuidGenerator(_number_generator);
    std::unordered_map<lnet::uuid::Uuid, Client> _clients;
    // keeps track of the verified UDP connections (linked with a TCP connection)
    // this is some kind of cache to find the client UUID from the UDP address fast
    std::map<lnet::net::SocketAddr, lnet::uuid::Uuid> _udp_connection_cache;
};

} // namespace net
