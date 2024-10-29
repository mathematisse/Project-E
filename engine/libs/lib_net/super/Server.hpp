
#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
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

/// This server class is a base class for a server that handles clients with both TCP and UDP
/// connections. UDP Connection are linked to TCP connections by a UUID.
/// All functions should be called from the same thread (the thread that calls update)
class Server : private lnet::utils::BaseServer {
public:
    struct Client {
        lnet::uuid::Uuid tcp_connection_id;
        lnet::net::SocketAddr udp_addr;
        std::uint64_t generated_number;

        Client(lnet::uuid::Uuid tcp_connection_id, std::uint64_t generated_number):
            tcp_connection_id(tcp_connection_id),
            generated_number(generated_number)
        {
        }
    };

    Server() = default;
    ~Server() override = default;

    virtual void on_tcp_connect(lnet::uuid::Uuid client_uuid) = 0;
    virtual void on_tcp_disconnect(lnet::uuid::Uuid client_uuid) = 0;

    virtual void on_udp_connect(lnet::uuid::Uuid client_uuid) = 0;
    virtual void on_packet(const Packet &packet, lnet::uuid::Uuid client_uuid) = 0;

protected:
    /// modifies the number given for security reasons (to prevent replay attacks)
    /// The function should be complex enough to prevent the client from guessing the function
    /// The function should not give the same result for different numbers
    virtual auto transformNumberFunction(std::uint64_t number) -> std::uint64_t;

public:
    auto host(std::uint16_t port
    ) -> lnet::result::Result<lnet::result::Void, lnet::utils::BaseServerError>;

    auto connect_tcp(const std::string &ip, std::uint16_t port)
        -> lnet::result::Result<lnet::result::Void, lnet::utils::BaseServerError>;

    auto connect_udp(const std::string &ip, std::uint16_t port)
        -> lnet::result::Result<lnet::result::Void, lnet::utils::BaseServerError>;

    auto start() -> decltype(start_context());
    inline auto stop() -> void { return stop_context(); }
    inline auto update() -> void { return handle_events(); }

    void send_tcp(
        lnet::uuid::Uuid client_uuid, Packet::MsgType type, const std::vector<std::uint8_t> &data
    );

    void send_udp(
        lnet::uuid::Uuid client_uuid, Packet::MsgType type, const std::vector<std::uint8_t> &data
    );

    void send_tcp_all(Packet::MsgType type, const std::vector<std::uint8_t> &data);

    void send_udp_all(Packet::MsgType type, const std::vector<std::uint8_t> &data);

    /*
        From BaseServer.hpp theses are transformed to higher order events common for TCP & UDP
        To be able to give the client_uuid to the user instead of the tcp_connection_id
    */
    void on_tcp_connection(lnet::uuid::Uuid tcp_connection_id) override;

    void on_tcp_disconnection(lnet::uuid::Uuid tcp_connection_id) override;

    /*
        on_tcp and on_udp have reserved space in the packets for lower level events that handle
        the udp connection request and the udp connection response
    */
    void
    on_udp_data(const lnet::net::SocketAddr &addr, const std::vector<std::uint8_t> &data) override;

    void on_tcp_data(
        lnet::uuid::Uuid tcp_connection_id,
        lnet::io::Mutex<lnet::io::BufReader<lnet::net::TcpStream>> &stream
    ) override;

    bool has_udp_connection(lnet::uuid::Uuid client_uuid) const;

    size_t numberClients() const { return _udp_connection_cache.size(); }

private:
#pragma pack(push, 1)
    struct UdpConnectionPacketInitialisation {
        lnet::uuid::Uuid client_uuid;
        std::uint64_t generated_number;
    };
#pragma pack(pop)

#pragma pack(push, 1)
    struct UdpConnectionPacketConfirmation {
        lnet::uuid::Uuid client_uuid;
        std::uint64_t transformed_number;
    };
#pragma pack(pop)

    /// The server sends a random number to the client
    void ask_udp_connection_request(
        lnet::uuid::Uuid client_uuid, std::uint64_t number, lnet::uuid::Uuid tcp_connection_id
    );

    /// The client receives the number
    void
    ask_udp_connection_response(lnet::uuid::Uuid client_uuid, std::uint64_t transformed_number);

    /// The client sends back the number transformed by the client
    void handle_udp_connection_request(
        const std::vector<std::uint8_t> &data, const lnet::net::SocketAddr &addr
    );

    /// The server receives the transformed number and checks if it is correct
    void handle_udp_connection_response(
        const std::vector<std::uint8_t> &data, const lnet::net::SocketAddr &addr
    );

    std::shared_ptr<std::mt19937_64> _number_generator =
        std::make_shared<std::mt19937_64>(std::random_device()());
    lnet::uuid::UuidGenerator<std::mt19937_64> client_uuid_generator =
        lnet::uuid::UuidGenerator(_number_generator);
    std::map<lnet::uuid::Uuid, Client> _clients;
    /// keeps track of the verified UDP connections (linked with a TCP connection)
    /// this is some kind of cache to find the client UUID from the UDP address fast
    std::map<lnet::net::SocketAddr, lnet::uuid::Uuid> _udp_connection_cache;
};

} // namespace net
