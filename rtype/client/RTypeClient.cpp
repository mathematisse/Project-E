#include "RTypeClient.hpp"
#include "GameEntities.hpp"
#include "lib_ecs/Components/PureComponentPools.hpp"
#include "lib_net/Packet.hpp"
#include "RTypePackets.hpp"
#include <cstdlib>

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
        if (newEnnemy.rand == 2) {
            auto ent = _entityManager.createEntity("GameAnimatedEntity", ECS::C::ENT_ALIVE);
            auto ref = _entityManager.getEntity(ent);
            auto *square_ennemy = dynamic_cast<ECS::E::GameAnimatedEntityRef *>(ref.get());
            if (square_ennemy == nullptr) {
                std::cerr << "Failed to cast IEntityRef to GameEntityRef" << std::endl;
                return;
            }
            square_ennemy->setHealth({5});
            square_ennemy->setWeapon({WeaponType::BIG_SHOT});
            square_ennemy->setSize({180, 120});
            square_ennemy->setAnimatedSprite({frigateSpriteId, 6, 0, 0.0F});
            square_ennemy->setVelocity({0.0F, 0.0F});
            square_ennemy->setType({GameEntityType::ENEMY});
            square_ennemy->setRotation({100.0F});
            square_ennemy->setColor({255, 0, 0, 255});
            square_ennemy->setSize({80, 80});
            square_ennemy->setRotation({90.0F});

            float _x = newEnnemy.x;
            float _y = newEnnemy.y;
            square_ennemy->setPosition({_x, _y});
            square_ennemy->setCanShoot({true, 1.5F, 0.0F});
            square_ennemy->setNetworkID({newEnnemy.netId});
            break;
        } else {
            auto ent = _entityManager.createEntity("GameEntity", ECS::C::ENT_ALIVE);
            auto ref = _entityManager.getEntity(ent);
            auto *square_ennemy = dynamic_cast<ECS::E::GameEntityRef *>(ref.get());
            if (square_ennemy == nullptr) {
                std::cerr << "Failed to cast IEntityRef to GameEntityRef" << std::endl;
                return;
            }
            square_ennemy->setHealth({2});
            square_ennemy->setWeapon({WeaponType::BULLET});
            square_ennemy->setSize({80, 80});
            square_ennemy->setSprite({ennemySpriteId});
            square_ennemy->setRotation({90});
            square_ennemy->setVelocity({0.0F, 0.0F});
            square_ennemy->setType({GameEntityType::ENEMY});
            square_ennemy->setRotation({100.0F});
            square_ennemy->setColor({255, 0, 0, 255});
            square_ennemy->setSize({80, 80});
            square_ennemy->setRotation({90.0F});

            float _x = newEnnemy.x;
            float _y = newEnnemy.y;
            square_ennemy->setPosition({_x, _y});
            square_ennemy->setCanShoot({true, 1.5F, 0.0F});
            square_ennemy->setNetworkID({newEnnemy.netId});
            break;
        }
    }
    case ECS::BULLET_SHOT: {
        auto bulletShot = *Packet::deserializeStruct<ECS::BulletShot>(packet.data);
        auto ent = _entityManager.createEntity("GameAnimatedEntity", ECS::C::ENT_ALIVE);
        auto ref = _entityManager.getEntity(ent);

        auto *square_bullet = dynamic_cast<ECS::E::GameAnimatedEntityRef *>(ref.get());
        if (square_bullet == nullptr) {
            std::cerr << "Failed to cast IEntityRef to GameEntityRef" << std::endl;
            return;
        }
        if (bulletShot.isPlayer) {
            square_bullet->setVelocity({500.0F, 0.0F});
            square_bullet->setRotation({90});
            square_bullet->setType({GameEntityType::BULLET});
        } else {
            square_bullet->setVelocity({-500.0F, 0.0F});
            square_bullet->setRotation({-90});
            square_bullet->setType({GameEntityType::BULLET_ENNEMY});
        }

        if (bulletShot.isBigShot) {
            square_bullet->setAnimatedSprite({bigShotSpriteId, 10, 0, 0.0F});
            square_bullet->setHealth({5});
            square_bullet->setSize({70, 70});
        } else {
            square_bullet->setAnimatedSprite({bulletSpriteId, 4, 0, 0.0F});
            square_bullet->setHealth({1});
            square_bullet->setSize({30, 30});
        }
        square_bullet->setPosition({bulletShot.x, bulletShot.y});
        square_bullet->setColor({255, 255, 0, 255});
        square_bullet->setCanShoot({false, 0.0F, 0.0F});
        square_bullet->setNetworkID({bulletShot.netId});
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
        auto *square_player = dynamic_cast<ECS::E::GameEntityRef *>(ent.get());
        if (square_player == nullptr) {
            std::cerr << "Failed to cast IEntityRef to GameEntityRef" << std::endl;
            return;
        }
        square_player->setNetworkID({playerConnectionSuccess.netId});
        square_player->setColor(
            {playerConnectionSuccess.r, playerConnectionSuccess.g, playerConnectionSuccess.b, 255}
        );
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
