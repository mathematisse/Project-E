#include "RTypeClient.hpp"
#include "Archetypes.hpp"
#include "lib_log/log.hpp"
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
        number = *reinterpret_cast<const std::uint64_t *>(packet.data.data());
        LOG_DEBUG("Received generated number " + std::to_string(number) + " from server: ");
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
            auto enemy = _entityManager.createEntity<ECS::E::AnimatedGameEntity>();
            enemy.setHealth({5});
            enemy.setWeapon({WeaponType::BIG_SHOT});
            enemy.setSize({180, 120});
            enemy.setAnimatedSprite({frigateSpriteId, 6, 0, 0.0F});
            enemy.setVelocity();
            enemy.setType({GameEntityType::ENEMY});
            enemy.setRotation({100.0F});
            enemy.setColor({255, 0, 0, 255});
            enemy.setSize({80, 80});
            enemy.setRotation({90.0F});
            enemy.setPosition({newEnnemy.x, newEnnemy.y});
            enemy.setCanShoot({true, 1.5F, 0.0F});
            enemy.setNetworkID({newEnnemy.netId});
            break;
        } else {
            auto enemy = _entityManager.createEntity<ECS::E::GameEntity>();
            enemy.setHealth({2});
            enemy.setWeapon({WeaponType::BULLET});
            enemy.setSize({80, 80});
            enemy.setSprite({enemySpriteId});
            enemy.setRotation({90});
            enemy.setVelocity();
            enemy.setType({GameEntityType::ENEMY});
            enemy.setRotation({100.0F});
            enemy.setColor({255, 0, 0, 255});
            enemy.setSize({80, 80});
            enemy.setRotation({90.0F});
            enemy.setPosition({newEnnemy.x, newEnnemy.y});
            enemy.setCanShoot({true, 1.5F, 0.0F});
            enemy.setNetworkID({newEnnemy.netId});
            break;
        }
    }
    case ECS::BULLET_SHOT: {
        auto bulletShot = *Packet::deserializeStruct<ECS::BulletShot>(packet.data);
        auto bullet = _entityManager.createEntity<ECS::E::AnimatedGameEntity>();
        LOG_DEBUG("Bullet shot with netId " + std::to_string(bulletShot.netId));
        if (bulletShot.isPlayer) {
            bullet.setVelocity({500.0F, 0.0F});
            bullet.setRotation({90});
            bullet.setType({GameEntityType::BULLET});
        } else {
            bullet.setVelocity({-500.0F, 0.0F});
            bullet.setRotation({-90});
            bullet.setType({GameEntityType::BULLET_ENNEMY});
        }
        if (bulletShot.isBigShot) {
            bullet.setAnimatedSprite({bigShotSpriteId, 10, 0, 0.0F});
            bullet.setHealth({5});
            bullet.setSize({70, 70});
        } else {
            bullet.setAnimatedSprite({bulletSpriteId, 4, 0, 0.0F});
            bullet.setHealth({1});
            bullet.setSize({30, 30});
        }
        bullet.setPosition({bulletShot.x, bulletShot.y});
        bullet.setColor({255, 255, 0, 255});
        bullet.setCanShoot({false, 0.0F, 0.0F});
        bullet.setNetworkID({bulletShot.netId});
        break;
    }
    case ECS::ENTITY_DESTROYED: {
        auto entityDestroyed = *Packet::deserializeStruct<ECS::EntityDestroyed>(packet.data);
        entitiesDestroyed.push_back(entityDestroyed);
        break;
    }
    case ECS::PLAYER_CONNECTION_SUCCESS: {
        auto playerConnectionSuccess =
            *Packet::deserializeStruct<ECS::PlayerConnectionSuccess>(packet.data);
        auto player = _entityManager.getEntity<ECS::E::GameEntity>(playerPos);
        player.setNetworkID({playerConnectionSuccess.netId});
        player.setColor(
            {playerConnectionSuccess.r, playerConnectionSuccess.g, playerConnectionSuccess.b, 255}
        );
        break;
    }
    case ECS::FRAME_ID: {
        auto frameId = *Packet::deserializeStruct<ECS::FrameId>(packet.data);
        cameraX = (80.0F * (float) frameId.frame * 0.02F) + (1920.0F / 2.0F);
        started = true;
        break;
    }
    case ECS::NEW_POWERUP: {
        auto newPowerUp = *Packet::deserializeStruct<ECS::NewPowerUp>(packet.data);
        auto powerUp = _entityManager.createEntity<ECS::E::AnimatedGameEntity>();
        float _x = newPowerUp.x;
        float _y = newPowerUp.y;
        powerUp.setType({GameEntityType::POWERUP});
        powerUp.setSize({80, 80});
        powerUp.setRotation({90});
        powerUp.setPosition({_x, _y});
        powerUp.setAnimatedSprite({powerUpSpriteId, 13, 0.0F, 0.0F});
        powerUp.setHealth({1});
        powerUp.setVelocity({0.0F, 0.0F});
        powerUp.setCanShoot({false, 0.0F, 0.0F});
        powerUp.setColor({255, 255, 0, 255});
        powerUp.setNetworkID({newPowerUp.netId});
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
