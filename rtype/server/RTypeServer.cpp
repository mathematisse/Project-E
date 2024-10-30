#include "RTypeServer.hpp"
#include "Archetypes.hpp"
#include "RTypePackets.hpp"
#include "lib_net/Packet.hpp"
#include "lib_log/log.hpp"
#include <string>

net::RTypeServer::RTypeServer(
    ECS::EntityManager &entityManager, NetworkManager &networkManager,
    std::vector<std::tuple<ECS::PlayerVelocityInput, size_t>> &playerStates,
    std::vector<client_id> &newClientIds
):
    _entityManager(entityManager),
    networkManager(networkManager),
    playerStates(playerStates),
    newClientIds(newClientIds)
{
}

void net::RTypeServer::on_tcp_connect(client_id id)
{
    LOG_INFO("Client connected: " + std::to_string(id));
    send_tcp(id, 255, {'h', 'e', 'l', 'l', 'o'});
    // get the generated number from the client
    try {
        auto number = clients.at(id).generated_number;
        auto number_interpreted_as_vector = std::vector<std::uint8_t>(
            reinterpret_cast<std::uint8_t *>(&number),
            reinterpret_cast<std::uint8_t *>(&number) + sizeof(number)
        );
        LOG_DEBUG(
            "Sending generated number (" + std::to_string(number) + ") to client " +
            std::to_string(id)
        );
        send_tcp(id, net::Packet::ASKUDP_NUMBER, number_interpreted_as_vector);
    } catch (const std::out_of_range &e) {
        LOG_ERROR("Client not found: " + std::to_string(id));
    }
}

void net::RTypeServer::on_udp_connect(client_id id)
{
    LOG_DEBUG("UDP client connected: " + std::to_string(id));

    auto player = _entityManager.createEntity<ECS::E::BaseEntity>();

    player.setPosition({1920.0F / 4.0F, 1080.0F / 2.0F});
    player.setType({GameEntityType::PLAYER});
    unsigned char r = rand() % 255;
    unsigned char g = rand() % 255;
    unsigned char b = rand() % 255;
    player.setColor({r, g, b, 255});
    player.setWeapon({WeaponType::BULLET});
    player.setCanShoot({true, (player.getWeaponVal() == WeaponType::BIG_SHOT) ? 1.5F : 0.3F, 0.0F});
    player.setSize({80, 80});
    player.setRotation({90});
    player.setHealth({4});

    auto netId = networkManager.getnewNetID();
    player.setNetworkID({netId});
    client_netIds[id] = netId;
    send_tcp(
        id, ECS::PLAYER_CONNECTION_SUCCESS,
        net::Packet::serializeStruct(ECS::PlayerConnectionSuccess {netId, r, g, b})
    );
    newClientIds.push_back(id);
}

void net::RTypeServer::on_tcp_disconnect(client_id id)
{
    LOG_INFO("Client disconnected: " + std::to_string(id));
}

void net::RTypeServer::on_packet(const Packet &packet, client_id id)
{
    // std::cout << "Received packet from client " << id << std::endl;
    // std::cout << "Packet size: " << packet.header.size << std::endl;
    switch (packet.header.type) {
    case Packet::PONG:
        LOG_DEBUG("Received PONG from client " + std::to_string(id));
        pong_count++;
        if (pong_count < 10) {
            LOG_DEBUG("Sending PING to client " + std::to_string(id));
            send_udp(id, Packet::PING, {'P', 'I', 'N', 'G'});
        }
        break;
    case ECS::PLAYER_VELOCITY:
        auto player_velocity = *Packet::deserializeStruct<ECS::PlayerVelocityInput>(packet.data);
        auto netId = client_netIds.at(id);
        for (auto &playerState : playerStates) {
            if (std::get<1>(playerState) == netId) {
                std::get<0>(playerState) = player_velocity;
                return;
            }
        }
        playerStates.emplace_back(player_velocity, netId);
        break;
    }
    if (packet.header.size == 0) {
        return;
    }
    // std::cout << "Packet data: ";
    // convert to string
    // std::string data(packet.data.begin(), packet.data.end());
    // std::cout << "\"" << data << "\"" << std::endl;
}

size_t net::RTypeServer::clientCount() const { return numberClients(); }