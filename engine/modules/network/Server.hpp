#pragma once

#include "lib_net/BaseServer.hpp"
#include "lib_net/Connection.hpp"
#include "lib_net/Packet.hpp"

#include <cstdint>
#include <iostream>
#include <memory>

enum class CustomMsgTypes : uint32_t {
    ServerAccept,
    ServerDeny,
    ServerPing,
    MessageAll,
    ServerMessage,
};

class CustomServer : public net::BaseServer<CustomMsgTypes> {
public:
    explicit CustomServer(uint16_t nPort):
        net::BaseServer<CustomMsgTypes>(nPort)
    {
    }

    ~CustomServer() override = default;
    CustomServer(const CustomServer &) = delete;
    CustomServer &operator=(const CustomServer &) = delete;
    CustomServer(CustomServer &&) = delete;
    CustomServer &operator=(CustomServer &&) = delete;

protected:
    bool on_client_connect(std::shared_ptr<net::Connection<net::Ownership::Server, CustomMsgTypes>> client
    ) override
    {
        net::Packet<CustomMsgTypes> packet;
        packet.header.type = CustomMsgTypes::ServerAccept;
        client->send(packet);
        return true;
    }

    void
    on_client_disconnect(std::shared_ptr<net::Connection<net::Ownership::Server, CustomMsgTypes>> /*client*/
    ) override
    {
        std::cout << "Removing client []\n";
    }

    // Called when a message arrives
    void on_packet(
        std::shared_ptr<net::Connection<net::Ownership::Server, CustomMsgTypes>> client,
        net::Packet<CustomMsgTypes> &packet
    ) override
    {
        switch (packet.header.type) {
        case CustomMsgTypes::ServerPing: {
            std::cout << "[]: Server Ping\n";

            // Simply bounce message back to client
            client->send(packet);
        } break;

        case CustomMsgTypes::MessageAll: {
            std::cout << "[]: Message All\n";

            // Construct a new message and send it to all clients
            net::Packet<CustomMsgTypes> packet;
            packet.header.type = CustomMsgTypes::ServerMessage;
            packet << 42;
            send_all_clients(packet, client);

        } break;
        case CustomMsgTypes::ServerAccept:
        case CustomMsgTypes::ServerDeny:
        case CustomMsgTypes::ServerMessage:
            break;
        }
    }
};