
#include "RTypePackets.hpp"
#include "RTypeServer.hpp"
#include "DecorEntities.hpp"
#include <cstdlib>
#include <chrono>
#include <vector>

// ECS includes
#include "GameEntities.hpp"
#include "Systems.hpp"
#include "ServerSystems.hpp"
#include "core/Core.hpp"
#include "lib_ecs/EntityManager.hpp"
#include "lib_ecs/Systems/SystemTree.hpp"
#include "lib_ecs/Components/PureComponentPools.hpp"
#include "lib_log/log.hpp"
#include "spatial2d/Spatial2D.hpp"

char player_is_alive(ECS::EntityManager &_eM, ECS::Chunks::cPosArr_t &chunks)
{
    auto player = chunks;
    if (player.empty()) {
        LOG_INFO("Player is dead");
        return 0;
    }
    auto ref = _eM.getEntity(player[0]);
    auto square_player = dynamic_cast<ECS::E::GameEntityRef *>(ref.get());
    if (!square_player) {
        LOG_ERROR("Failed to cast IEntityRef to GameEntityRef");
        return 0;
    }
    return *square_player->getHealth().get<0>();
}

ECS::Chunks::cPosArr_t setup_player(ECS::EntityManager &_eM, NetworkManager &networkManager)
{
    auto player = _eM.createEntities("GameEntity", 1, ECS::C::ENT_ALIVE);

    for (const auto &entity : player) {
        auto ref = _eM.getEntity(entity);

        auto *square_player = dynamic_cast<ECS::E::GameEntityRef *>(ref.get());
        if (square_player == nullptr) {
            LOG_ERROR("Failed to cast IEntityRef to GameEntityRef");
            return {};
        }
        square_player->setPosition({1920 / 4, 1080 / 2});
        square_player->setType({GameEntityType::PLAYER});
        square_player->setColor({255, 0, 0, 255});
        square_player->setWeapon({WeaponType::BULLET});
        square_player->setCanShoot(
            {true, *square_player->getWeapon().get<0>() == WeaponType::BIG_SHOT ? 1.5F : 0.3F, 0.0F}
        );
        square_player->setSize({80, 80});
        square_player->setRotation({90});
        square_player->setSprite({0});
        square_player->setHealth({4});
        square_player->setNetworkID({networkManager.getnewNetID()});
    }
    return player;
}

int main(int ac, char **av)
{
    LOG_SET_FILE("rtype_server.log");
    LOG_SET_LEVEL(DEBUG);

    ECS::S::MovePlayersSystem movePlayersSystem;
    ECS::EntityManager _eM(FIXED_TIMESTEP);
    NetworkManager networkManager;
    ECS::S::SendAllDataToNewClients sendAllDataToNewClients;
    net::RTypeServer server(
        _eM, networkManager, movePlayersSystem.playerStates, sendAllDataToNewClients.newClients
    );
    sendAllDataToNewClients.server = &server;
    std::uint16_t port = 0;

    if (ac != 2) {
        std::cerr << "Usage: ./rtype_server port" << std::endl;
        return 1;
    }
    port = std::stoi(av[1]);

    server.host(port);
    std::cout << "Server started on port " << port << std::endl;
    srand(time(NULL));

    float cameraX = 1920 / 2;

    // Engine modules
    engine::module::Core mCore;
    mCore.load(_eM);
    engine::module::Spatial2D mSpatial2D;
    mSpatial2D.load(_eM);

    ECS::S::SpawnEnnemySystem spawnEnnemySystem(_eM, networkManager, 0, server, 5);
    ECS::S::DestroyEntitiesSystem destroyEntitiesSystem(_eM, server);
    ECS::S::ShootSystem shootSystem(_eM, networkManager, 0, server);
    ECS::S::MoveBackgroundSystem moveBackgroundSystem;
    ECS::S::MoveEnnemySystem moveEnnemySystem;
    ECS::S::ColliderSystem colliderSystem;
    ECS::S::CountEnnemyAliveSystem countEnnemyAliveSystem(spawnEnnemySystem.ennemyCount);
    ECS::S::GetPlayerPositionSystem getPlayerPositionSystem;

    // Entity pools
    ECS::E::GameAnimatedEntityPool gameAnimatedEntityPool;
    _eM.registerEntityPool(&gameAnimatedEntityPool);
    ECS::E::GameEntityPool gameEntityPool;
    _eM.registerEntityPool(&gameEntityPool);

    ECS::S::SystemTreeNode rTypeFixedNode(
        "RTypeFixedNode", {&spawnEnnemySystem, &countEnnemyAliveSystem},
        {&moveBackgroundSystem, &moveEnnemySystem, &movePlayersSystem, &shootSystem,
         &colliderSystem, &destroyEntitiesSystem, &getPlayerPositionSystem,
         &sendAllDataToNewClients}
    );
    _eM.registerFixedSystemNode(rTypeFixedNode, ROOT_SYS_GROUP, false, true);

    shootSystem.deltaTime = 0.02F;

    auto curr_time = std::chrono::steady_clock::now();

    size_t frame = 0;
    bool started = false;

    while (true) {
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        server.update();
        auto new_time = std::chrono::steady_clock::now();
        auto dt = std::chrono::duration<float>(new_time - curr_time).count();
        curr_time = new_time;

        moveEnnemySystem.playersPos = getPlayerPositionSystem.playersPos;
        shootSystem.playersPos = getPlayerPositionSystem.playersPos;
        getPlayerPositionSystem.playersPos.clear();

        countEnnemyAliveSystem.ennemyCount = 0;
        if ((server.clientCount() > 1) || started) {
            cameraX += 80 * dt;
            moveBackgroundSystem.cameraX = cameraX;
            started = true;
            if (_eM.addTime(dt)) {
                frame++;
                sendAllDataToNewClients.newClients.clear();
                movePlayersSystem.playerStates.clear();
                server.send_tcp(ECS::FRAME_ID, net::Packet::serializeStruct(ECS::FrameId {frame}));
                spawnEnnemySystem.ennemyCount = countEnnemyAliveSystem.ennemyCount;
            }
        }
    }
    return 0;
}