
#include "RTypePackets.hpp"
#include "RTypeServer.hpp"
#include "Archetypes.hpp"
#include <cstdlib>
#include <chrono>
#include <vector>

// ECS includes
#include "Systems.hpp"
#include "ServerSystems.hpp"
#include "core/Core.hpp"
#include "lib_ecs/Chunks/ChunkPos.hpp"
#include "lib_ecs/EntityManager.hpp"
#include "lib_ecs/Systems/SystemTree.hpp"
#include "lib_log/log.hpp"
#include "spatial2d/Spatial2D.hpp"

bool player_is_alive(ECS::EntityManager &_eM, ECS::Chunks::chunkPos_t &player)
{
    return _eM.getEntity<ECS::E::BaseEntity>(player).getHealthVal() > 0;
}

ECS::Chunks::chunkPos_t setup_player(ECS::EntityManager &_eM, NetworkManager &networkManager)
{
    auto player = _eM.createEntity<ECS::E::BaseEntity>();

    player.setPosition({1920.0F / 4, 1080.0F / 2});
    player.setType({GameEntityType::PLAYER});
    player.setColor({255, 0, 0, 255});
    player.setWeapon({WeaponType::BULLET});
    player.setCanShoot({true, player.getWeaponVal() == WeaponType::BIG_SHOT ? 1.5F : 0.3F, 0.0F});
    player.setSize({80, 80});
    player.setRotation({90});
    player.setHealth({4});
    player.setNetworkID({networkManager.getnewNetID()});
    return player.getChunkPosVal();
}

int main(int ac, char **av)
{
    LOG_SET_LEVEL(DEBUG);
    LOG_SET_STREAM(std::cerr);
    LOG_SET_FILE("rtype_server.log", true);

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
    srand(time(nullptr));

    float cameraX = 1920.0F / 2;

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
    ECS::S::CountEnnemyAliveSystem countEnnemyAliveSystem(spawnEnnemySystem.enemyCount);
    ECS::S::GetPlayerPositionSystem getPlayerPositionSystem;

    // Entity pools
    ECS::E::BaseEntity::Pool gameEntityPool(RTYPE_ENTITY_POOL_SIZE);
    _eM.registerEntityPool(&gameEntityPool);

    ECS::S::SystemTreeNode rTypeFixedNode(
        "RTypeFixedNode", {&spawnEnnemySystem, &countEnnemyAliveSystem},
        {&moveBackgroundSystem, &moveEnnemySystem, &movePlayersSystem, &shootSystem,
         &colliderSystem, &destroyEntitiesSystem, &getPlayerPositionSystem,
         &sendAllDataToNewClients}
    );
    bool success = _eM.registerFixedSystemNode(rTypeFixedNode, ROOT_SYS_GROUP);

    if (success) {
        LOG_INFO("Successfully registered RTypeFixedNode");
    } else {
        LOG_ERROR("Failed to register RTypeFixedNode");
    }

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

        countEnnemyAliveSystem.enemyCount = 0;
        if ((server.clientCount() > 1) || started) {
            cameraX += 80 * dt;
            moveBackgroundSystem.cameraX = cameraX;
            started = true;
            if (_eM.addTime(dt)) {
                frame++;
                sendAllDataToNewClients.newClients.clear();
                movePlayersSystem.playerStates.clear();
                moveEnnemySystem.playersPos = getPlayerPositionSystem.playersPos;
                shootSystem.playersPos = getPlayerPositionSystem.playersPos;
                getPlayerPositionSystem.playersPos.clear();
                server.send_tcp(ECS::FRAME_ID, net::Packet::serializeStruct(ECS::FrameId {frame}));
                spawnEnnemySystem.enemyCount = countEnnemyAliveSystem.enemyCount;
            }
        }
    }
    return 0;
}