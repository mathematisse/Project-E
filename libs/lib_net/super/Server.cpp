
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <map>
#include <optional>
#include <span>
#include <vector>

#include "lib_log/log.hpp"
#include "lib_net/Packet.hpp"
#include "lib_net/net/SocketAddr.hpp"
#include "lib_net/net/TcpStream.hpp"
#include "lib_net/super/BaseServer.hpp"
#include "lib_net/uuid/Uuid.hpp"
#include "lib_net/super/Server.hpp"

namespace net {
// modifies the number given for security reasons (to prevent replay attacks)
// The function should be complex enough to prevent the client from guessing the function
// The function should not give the same result for different numbers
auto Server::transformNumberFunction(std::uint64_t number) -> std::uint64_t
{
    // example function (should be overriden by the higher order class)
    constexpr std::uint64_t SECRET_NUMBER = 0x1234567890ABCDEF;
    return number ^ SECRET_NUMBER;
}

// using the same port for both TCP and UDP (for simplicity)
auto Server::host(std::uint16_t port
) -> lnet::result::Result<lnet::result::Void, lnet::utils::BaseServerError>
{
    if (auto res = host_tcp(port); !res) {
        return res;
    }
    LOG_DEBUG("TCP hosted on port " + std::to_string(port));
    return host_udp(port);
}

auto Server::connect_tcp(const std::string &ip, std::uint16_t port)
    -> lnet::result::Result<lnet::result::Void, lnet::utils::BaseServerError>
{
    LOG_DEBUG("Connecting TCP to " + ip + ":" + std::to_string(port));
    return lnet::utils::BaseServer::connect_tcp(ip, port);
}

auto Server::connect_udp(const std::string &ip, std::uint16_t port)
    -> lnet::result::Result<lnet::result::Void, lnet::utils::BaseServerError>
{
    LOG_DEBUG("Connecting UDP to " + ip + ":" + std::to_string(port));
    return lnet::utils::BaseServer::connect_udp(ip, port);
}

auto Server::start() -> decltype(start_context())
{
    LOG_DEBUG("Server started");
    return start_context();
}

void Server::send_tcp(
    lnet::uuid::Uuid client_uuid, Packet::MsgType type, const std::vector<std::uint8_t> &data
)
{
    LOG_DEBUG("Sending TCP to client " + client_uuid.to_str());
    auto matchingClient = _clients.find(client_uuid);
    Packet packet = Packet::deserialize(type, data);
    if (matchingClient != _clients.end()) {
        lnet::utils::BaseServer::send_tcp(
            matchingClient->second.tcp_connection_id, packet.serialize()
        );
    }
}

void Server::send_udp(
    lnet::uuid::Uuid client_uuid, Packet::MsgType type, const std::vector<std::uint8_t> &data
)
{
    auto matchingClient = _clients.find(client_uuid);
    Packet packet = Packet::deserialize(type, data);
    if (matchingClient != _clients.end()) {
        lnet::utils::BaseServer::send_udp(matchingClient->second.udp_addr, packet.serialize());
    } else {
        std::cout << "Client not found" << std::endl;
    }
}

void Server::send_tcp_all(Packet::MsgType type, const std::vector<std::uint8_t> &data)
{
    Packet packet = Packet::deserialize(type, data);
    for (const auto &[client_uuid, client] : _clients) {
        lnet::utils::BaseServer::send_tcp(client.tcp_connection_id, packet.serialize());
    }
}

void Server::send_udp_all(Packet::MsgType type, const std::vector<std::uint8_t> &data)
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
void Server::on_tcp_connection(lnet::uuid::Uuid tcp_connection_id)
{
    auto client_uuid = client_uuid_generator.new_uuid();
    auto number = static_cast<std::uint64_t>((*_number_generator)());

    // append a new connection to the list
    _clients.insert({client_uuid, Client {tcp_connection_id, number}});
    on_tcp_connect(client_uuid);
    ask_udp_connection_request(client_uuid, number, tcp_connection_id);
}

void Server::on_tcp_disconnection(lnet::uuid::Uuid tcp_connection_id)
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
void Server::on_udp_data(const lnet::net::SocketAddr &addr, const std::vector<std::uint8_t> &data)
{
    // if the client is already connected with UDP on_packet will be called else we call
    // handle_udp_connection_request
    if (auto matchingClient = _udp_connection_cache.find(addr);
        matchingClient != _udp_connection_cache.end() || _udp_confirmed) {
        auto packet = Packet::deserialize(data);
        if (packet.has_value()) {
            on_packet(packet.value(), matchingClient->second);
        }
    } else {
        handle_udp_connection_request(data, addr);
    }
}

// TODO: remove stream reference (edge case exist where the stream is deleted in another thread)
void Server::on_tcp_data(
    lnet::uuid::Uuid tcp_connection_id,
    lnet::io::Mutex<lnet::io::BufReader<lnet::net::TcpStream>> &stream
)
{
    auto matchingClient =
        std::find_if(_clients.begin(), _clients.end(), [tcp_connection_id](const auto &client) {
            return client.second.tcp_connection_id == tcp_connection_id;
        });
    if (matchingClient != _clients.end()) {
        std::optional<Packet> packet;
        std::span<std::uint8_t> buffer;
        stream.with_lock([&buffer, &packet](auto &reader) {
            buffer = reader.buffer();
            packet = Packet::deserialize(buffer);
        });

        while (packet.has_value()) {
            if (packet->header.type == Packet::SystemTypes::ASKUDP_NUMBER) {
                if (auto udp_data =
                        Packet::deserializeStruct<UdpConnectionPacketInitialisation>(packet->data);
                    udp_data.has_value()) {
                    LOG_DEBUG("UDP connection request received");
                    ask_udp_connection_response(
                        udp_data->client_uuid, transformNumberFunction(udp_data->generated_number)
                    );
                }
            } else {
                on_packet(packet.value(), matchingClient->first);
            }
            stream.with_lock([&buffer, &packet](auto &reader) {
                reader.consume(packet->size());
                buffer = reader.buffer();
            });
            packet = Packet::deserialize(buffer);
        }
    } else {
        std::cerr << "Client for packet not found" << std::endl;
    }
}

bool Server::has_udp_connection(lnet::uuid::Uuid client_uuid) const
{
    auto matchingClient = _clients.find(client_uuid);
    if (matchingClient != _clients.end()) {
        return _udp_connection_cache.find(matchingClient->second.udp_addr) !=
            _udp_connection_cache.end();
    }
    return false;
}

//////////////////////////////////////////////////////////////////////////////

// ask the client to send the UDP connection request (tell the client that the server is ready
// to take UDP connection requests)
// give the client UUID and some random number to the user, and expect the client to send the
// UUID back as a tranformed number
void Server::ask_udp_connection_request(
    lnet::uuid::Uuid client_uuid, std::uint64_t number, lnet::uuid::Uuid tcp_connection_id
)
{
    Packet packet = Packet::deserialize(
        Packet::SystemTypes::ASKUDP_NUMBER,
        Packet::serializeStruct(UdpConnectionPacketInitialisation {client_uuid, number})
    );
    LOG_DEBUG("Asking UDP connection to client " + client_uuid.to_str());
    lnet::utils::BaseServer::send_tcp(tcp_connection_id, packet.serialize());
}

void Server::ask_udp_connection_response(
    lnet::uuid::Uuid client_uuid, std::uint64_t transformed_number
)
{
    // send to the server this server is connected to
    /// client_uuid is the uuid of the receiver (the one who calls this function)
    auto data = Packet::deserialize(
        Packet::SystemTypes::ASKUDP_RESPONSE,
        Packet::serializeStruct(UdpConnectionPacketConfirmation {client_uuid, transformed_number})
    );
    lnet::utils::BaseServer::send_udp(data.serialize());
}

// First connection
void Server::handle_udp_connection_request(
    const std::vector<std::uint8_t> &data, const lnet::net::SocketAddr &addr
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

} // namespace net
