#include "RTypeClient.hpp"
#include "Square.hpp"
#include "lib_ecs/Components/PureComponentPools.hpp"
#include "lib_net/Packet.hpp"
#include "RTypePackets.hpp"

net::RTypeClient::RTypeClient(
    ECS::EntityManager &entityManager, std::vector<ECS::PlayerState> &playerStates,
    std::vector<ECS::EntityDestroyed> &entitiesDestroyed, float &cameraX
):
    cameraX(cameraX),
    _entityManager(entityManager),
    playerStates(playerStates),
    entitiesDestroyed(entitiesDestroyed)
{
}

void net::RTypeClient::on_packet(const Packet &packet)
{
    std::uint64_t transformed_number;
    std::uint64_t number;

    switch (packet.header.type) {
    case Packet::ASKUDP_NUMBER:
        std::cout << "Received generated number from server: ";
        number = *reinterpret_cast<const std::uint64_t *>(packet.data.data());
        std::cout << number << std::endl;
        // send the number back to the server with the function transformNumberFunction applied
        transformed_number = Gateway::transformNumberFunction(number);
        send_udp(Packet::ASKUDP_RESPONSE, Packet::serializeStruct(transformed_number));
        break;
    case Packet::PING:
        send_udp(Packet::PONG, {});
        break;
    case ECS::PLAYER_STATE: // player state
    {
        auto pState = *Packet::deserializeStruct<ECS::PlayerState>(packet.data);
        for (auto &playerState : playerStates) {
            auto [x, vx, y, vy, health, pNetId] = playerState;
            if (pNetId == pState.netId) {
                playerState = pState;
                return;
            }
        }
        playerStates.push_back(pState);
        break;
    }
    case ECS::NEW_ENNEMY: {
        auto newEnnemy = *Packet::deserializeStruct<ECS::NewEnnemy>(packet.data);
        auto ent = _entityManager.createEntity("Square", ECS::C::ENT_ALIVE);
        auto ref = _entityManager.getEntity(ent);
        auto *square_ennemy = dynamic_cast<ECS::E::SquareRef *>(ref.get());
        if (square_ennemy == nullptr) {
            std::cerr << "Failed to cast IEntityRef to SquareRef" << std::endl;
            return;
        }
        square_ennemy->getVelocity()->set<0>(0.0F);
        square_ennemy->getVelocity()->set<1>(0.0F);
        square_ennemy->getVelocity()->set<2>(150.0F);
        square_ennemy->getType()->set<0>(SquareType::ENEMY);
        square_ennemy->getColor()->set<0>(255);
        square_ennemy->getColor()->set<1>(0);
        square_ennemy->getColor()->set<2>(0);
        square_ennemy->getColor()->set<3>(255);
        square_ennemy->getSize()->set<0>(80);
        square_ennemy->getSize()->set<1>(80);
        square_ennemy->getSize()->set<2>(90);

        float _x = newEnnemy.x;
        float _y = newEnnemy.y;
        square_ennemy->getPosition()->set<0>(_x);
        square_ennemy->getPosition()->set<1>(_y);
        square_ennemy->getCanShoot()->set<0>(true);
        square_ennemy->getCanShoot()->set<1>(1.5F);
        square_ennemy->getSprite()->set<0>(ennemySpriteId);
        square_ennemy->getHealth()->set<0>(2);
        square_ennemy->getNetworkID()->set<0>(newEnnemy.netId);
        break;
    }
    case ECS::BULLET_SHOT: {
        auto bulletShot = *Packet::deserializeStruct<ECS::BulletShot>(packet.data);
        auto ent = _entityManager.createEntity("Square", ECS::C::ENT_ALIVE);
        auto ref = _entityManager.getEntity(ent);

        auto *square_bullet = dynamic_cast<ECS::E::SquareRef *>(ref.get());
        if (square_bullet == nullptr) {
            std::cerr << "Failed to cast IEntityRef to SquareRef" << std::endl;
            return;
        }
        if (bulletShot.isPlayer) {
            square_bullet->getVelocity()->set<0>(1.0F);
            square_bullet->getSize()->set<2>(90);
            square_bullet->getType()->set<0>(SquareType::BULLET);
        } else {
            square_bullet->getVelocity()->set<0>(-1.0F);
            square_bullet->getSize()->set<2>(-90);
            square_bullet->getType()->set<0>(SquareType::BULLET_ENNEMY);
        }
        square_bullet->getPosition()->set<0>(bulletShot.x);
        square_bullet->getVelocity()->set<1>(0.0F);
        square_bullet->getVelocity()->set<2>(500.0F);
        square_bullet->getColor()->set<0>(255);
        square_bullet->getColor()->set<1>(255);
        square_bullet->getColor()->set<2>(0);
        square_bullet->getColor()->set<3>(255);
        square_bullet->getPosition()->set<1>(bulletShot.y);
        square_bullet->getCanShoot()->set<0>(false);
        square_bullet->getSize()->set<0>(30);
        square_bullet->getSize()->set<1>(30);
        square_bullet->getSprite()->set<0>(bulletSpriteId);
        square_bullet->getSprite()->set<1>(true);
        square_bullet->getSprite()->set<2>(30.0F);
        square_bullet->getSprite()->set<3>(30.0F);
        square_bullet->getSprite()->set<4>(4.0F);
        square_bullet->getSprite()->set<5>(0);
        square_bullet->getHealth()->set<0>(1);
        square_bullet->getTimer()->set<0>(0.0F);
        square_bullet->getTimer()->set<1>(8.0F);
        square_bullet->getNetworkID()->set<0>(bulletShot.netId);
        break;
    }
    case ECS::ENTITY_DESTROYED: {
        auto entityDestroyed = *Packet::deserializeStruct<ECS::EntityDestroyed>(packet.data);
        for (auto &_entityDestroyed : entitiesDestroyed) {
            if (_entityDestroyed.netId == entityDestroyed.netId) {
                return;
            }
        }
        entitiesDestroyed.push_back(entityDestroyed);
        break;
    }
    case ECS::PLAYER_CONNECTION_SUCCESS: {
        auto playerConnectionSuccess =
            *Packet::deserializeStruct<ECS::PlayerConnectionSuccess>(packet.data);
        auto ent = _entityManager.getEntity(playerPos);
        auto *square_player = dynamic_cast<ECS::E::SquareRef *>(ent.get());
        if (square_player == nullptr) {
            std::cerr << "Failed to cast IEntityRef to SquareRef" << std::endl;
            return;
        }
        square_player->getNetworkID()->set<0>(playerConnectionSuccess.netId);
        square_player->getColor()->set<0>(playerConnectionSuccess.r);
        square_player->getColor()->set<1>(playerConnectionSuccess.g);
        square_player->getColor()->set<2>(playerConnectionSuccess.b);
        square_player->getColor()->set<3>(255);
        break;
    }
    case ECS::FRAME_ID: {
        auto frameId = *Packet::deserializeStruct<ECS::FrameId>(packet.data);
        cameraX = (80 * frameId.frame * 0.02F) + (1920 / 2);
        started = true;
        break;
    }
    default:
        // std::cout << "Received packet from server" << std::endl;
        // std::cout << "Packet size: " << packet.header.size << std::endl;
        if (packet.header.size == 0) {
            return;
        }
        // std::cout << "Packet data: ";
        // convert to string
        std::string data(packet.data.begin(), packet.data.end());
        // std::cout << "\"" << data << "\"" << std::endl;
    }
}