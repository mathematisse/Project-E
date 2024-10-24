#include "RTypeServer.hpp"
#include "DecorEntities.hpp"
#include "RTypePackets.hpp"
#include "GameEntities.hpp"
#include "lib_net/Packet.hpp"
#include <iostream>

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
    std::cout << "Client connected: " << id << std::endl;
    send_tcp(id, 255, {'h', 'e', 'l', 'l', 'o'});
    // get the generated number from the client
    try {
        auto number = clients.at(id).generated_number;
        auto number_interpreted_as_vector = std::vector<std::uint8_t>(
            reinterpret_cast<std::uint8_t *>(&number),
            reinterpret_cast<std::uint8_t *>(&number) + sizeof(number)
        );
        std::cout << "Sending generated number (" << number << ") to client " << id << std::endl;
        send_tcp(id, net::Packet::ASKUDP_NUMBER, number_interpreted_as_vector);
    } catch (const std::out_of_range &e) {
        std::cerr << "Client not found: " << id << std::endl;
    }
}

void net::RTypeServer::on_udp_connect(client_id id)
{
    std::cout << "UDP client connected: " << id << std::endl;

    auto newP = _entityManager.createEntities("GameEntity", 1, ECS::C::ENT_ALIVE);
    for (const auto &entity : newP) {
        auto ref = _entityManager.getEntity(entity);

        auto *square_player = dynamic_cast<ECS::E::GameEntityRef *>(ref.get());
        if (square_player == nullptr) {
            std::cerr << "Failed to cast IEntityRef to GameEntityRef" << std::endl;
            return;
        }
        square_player->setPosition({1920.0F / 4.0F, 1080.0F / 2.0F});
        square_player->setType({GameEntityType::PLAYER});
        unsigned char r = rand() % 255;
        unsigned char g = rand() % 255;
        unsigned char b = rand() % 255;
        square_player->setColor({r, g, b, 255});
        square_player->setWeapon({WeaponType::BULLET});
        square_player->setCanShoot(
            {true, (*square_player->getWeapon().get<0>() == WeaponType::BIG_SHOT) ? 1.5F : 0.3F,
             0.0F}
        );
        square_player->setSize({80, 80});
        square_player->setRotation({90});
        square_player->setSprite({0});
        square_player->setHealth({4});
        auto netId = networkManager.getnewNetID();
        square_player->setNetworkID({netId});

        client_netIds[id] = netId;

        send_tcp(
            id, ECS::PLAYER_CONNECTION_SUCCESS,
            net::Packet::serializeStruct(ECS::PlayerConnectionSuccess {netId, r, g, b})
        );
    }
    newClientIds.push_back(id);
}

void net::RTypeServer::on_tcp_disconnect(client_id id)
{
    std::cout << "Client disconnected: " << id << std::endl;
}

void net::RTypeServer::on_packet(const Packet &packet, client_id id)
{
    // std::cout << "Received packet from client " << id << std::endl;
    // std::cout << "Packet size: " << packet.header.size << std::endl;
    switch (packet.header.type) {
    case Packet::PONG:
        std::cout << "Received PONG from client " << id << std::endl;
        pong_count++;
        if (pong_count < 10) {
            std::cout << "Sending PING to client " << id << std::endl;
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

size_t net::RTypeServer::clientCount() const { return clients.size(); }