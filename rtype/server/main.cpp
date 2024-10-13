
#include "network/TestServer.hpp"
#include "DecorSquare.hpp"
#include <cstdlib>
#include <chrono>

// ECS includes
#include "Square.hpp"
#include "Systems.hpp"
#include "lib_ecs/EntityManager.hpp"
#include "lib_ecs/Systems/SystemTree.hpp"
#include "lib_ecs/Components/PureComponentPools.hpp"

Vector2 get_player_position(ECS::EntityManager &_eM, ECS::Chunks::cPosArr_t &chunks)
{
    auto player = chunks;
    if (player.empty()) {
        return {0, 0};
    }
    auto ref = _eM.getEntity(player[0]);
    auto square_player = dynamic_cast<ECS::E::SquareRef *>(ref.get());
    if (!square_player) {
        std::cerr << "Failed to cast IEntityRef to SquareRef" << std::endl;
        return {0, 0};
    }
    return {*square_player->getPosition()->get<0>(), *square_player->getPosition()->get<1>()};
}

char player_is_alive(ECS::EntityManager &_eM, ECS::Chunks::cPosArr_t &chunks)
{
    auto player = chunks;
    if (player.empty()) {
        std::cout << "Player is dead" << std::endl;
        return 0;
    }
    auto ref = _eM.getEntity(player[0]);
    auto square_player = dynamic_cast<ECS::E::SquareRef *>(ref.get());
    if (!square_player) {
        std::cerr << "Failed to cast IEntityRef to SquareRef" << std::endl;
        return 0;
    }
    return *square_player->getHealth()->get<0>();
}

ECS::Chunks::cPosArr_t setup_player(ECS::EntityManager &_eM, NetworkManager &networkManager)
{
    auto player = _eM.createEntities("Square", 1, ECS::C::ENT_ALIVE);

    for (const auto &entity : player) {
        auto ref = _eM.getEntity(entity);

        auto *square_player = dynamic_cast<ECS::E::SquareRef *>(ref.get());
        if (square_player == nullptr) {
            std::cerr << "Failed to cast IEntityRef to SquareRef" << std::endl;
            return {};
        }
        square_player->getPosition()->set<0>(1920 / 4);
        square_player->getPosition()->set<1>(1080 / 2);
        square_player->getVelocity()->set<2>(300.0F);
        square_player->getType()->set<0>(SquareType::PLAYER);
        square_player->getColor()->set<1>(255);
        square_player->getColor()->set<3>(255);
        square_player->getCanShoot()->set<0>(true);
        square_player->getCanShoot()->set<1>(0.3F);
        square_player->getSize()->set<0>(80);
        square_player->getSize()->set<1>(80);
        square_player->getSize()->set<2>(90);
        square_player->getSprite()->set<0>(0);
        square_player->getHealth()->set<0>(4);
        square_player->getNetworkID()->set<0>(networkManager.getnewNetID());
    }
    return player;
}

int main(int ac, char **av)
{
    srand(time(NULL));

    float cameraX = 1920 / 2;

    NetworkManager networkManager;

    ECS::EntityManager _eM;

    ECS::S::ApplyVelocitySystem applyVelocitySystem;
    ECS::S::SpawnEnnemySystem spawnEnnemySystem(_eM, networkManager, 0);
    ECS::S::DestroyEntitiesSystem destroyEntitiesSystem(_eM);
    ECS::S::ShootSystem shootSystem(_eM, networkManager, 0);
    ECS::S::MoveBackgroundSystem moveBackgroundSystem;
    ECS::S::MoveEnnemySystem moveEnnemySystem;
    ECS::S::ColliderSystem colliderSystem;
    ECS::S::CountEnnemyAliveSystem countEnnemyAliveSystem(spawnEnnemySystem.ennemyCount);

    // Entity pools
    ECS::E::SquarePool squarePool;
    ECS::E::DecorSquarePool decorSquarePool;

    ECS::S::SystemTreeNode demoFixedNode(
        42, {&spawnEnnemySystem, &countEnnemyAliveSystem},
        {&moveBackgroundSystem, &moveEnnemySystem, &applyVelocitySystem, &shootSystem, &colliderSystem,
         &destroyEntitiesSystem}
    );

    _eM.registerFixedSystemNode(demoFixedNode, ECS::S::ROOTSYSGROUP, false, true);

    _eM.registerEntityPool(&decorSquarePool);
    _eM.registerEntityPool(&squarePool);

    auto player = setup_player(_eM, networkManager);

    Vector2 playerPosition = {0, 0};

    applyVelocitySystem.deltaTime = 0.02F;
    shootSystem.deltaTime = 0.02F;

    auto curr_time = std::chrono::steady_clock::now();

    size_t frame = 0;

    net::TestServer server;
    std::uint16_t port = 0;

    if (ac != 2) {
        std::cerr << "Usage: ./rtype_server port" << std::endl;
        return 1;
    }
    port = std::stoi(av[1]);

    server.host(port);
    std::cout << "Server started on port " << port << std::endl;
    while (true) {
        server.update();
    }
    auto new_time = std::chrono::steady_clock::now();
    auto dt = std::chrono::duration<float>(new_time - curr_time).count();
    curr_time = new_time;

    cameraX += 80 * dt;
    moveBackgroundSystem.cameraX = cameraX;
    playerPosition = get_player_position(_eM, player);
    moveEnnemySystem.playerPosition = playerPosition;
    shootSystem.playerPosition = playerPosition;

    countEnnemyAliveSystem.ennemyCount = 0;
    frame += static_cast<size_t>(_eM.addTime(dt));
    spawnEnnemySystem.ennemyCount = countEnnemyAliveSystem.ennemyCount;
}
return 0;
}