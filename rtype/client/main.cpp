#include "DecorEntities.hpp"
#include <cstdlib>
#include <chrono>

#include <iostream>
#include <raylib.h>
#include <thread>
#include "AssetsPath.hpp"

// ECS includes
#include "lib_ecs/Chunks/ChunkPos.hpp"
#include "lib_ecs/Components/PureComponentPools.hpp"
#include "GameEntities.hpp"
#include "Systems.hpp"
#include "ClientSystems.hpp"
#include "DrawSystems.hpp"
#include "lib_ecs/EntityManager.hpp"
#include "lib_ecs/Systems/SystemTree.hpp"
#include "RTypeClient.hpp"
#include "render/Render.hpp"
#include "spatial2d/Spatial2D.hpp"


void init_camera(Camera2D &camera)
{
    camera.target = {1920 / 2, 1080 / 2};
    camera.offset = {1920 / 2.0f, 1080 / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

void update_camera(Camera2D &camera, float dt)
{
    Vector2 old = camera.target;
    camera.target = {old.x + 80 * dt, 1080 / 2};
}

Vector2 get_player_position(ECS::EntityManager &_eM, ECS::Chunks::cPosArr_t &chunks)
{
    auto player = chunks;
    if (player.empty()) {
        return {0, 0};
    }
    auto ref = _eM.getEntity(player[0]);
    auto square_player = dynamic_cast<ECS::E::GameEntityRef *>(ref.get());
    if (!square_player) {
        std::cerr << "Failed to cast IEntityRef to GameEntityRef" << std::endl;
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
    auto square_player = dynamic_cast<ECS::E::GameEntityRef *>(ref.get());
    if (!square_player) {
        std::cerr << "Failed to cast IEntityRef to GameEntityRef" << std::endl;
        return 0;
    }
    return *square_player->getHealth()->get<0>();
}

void update_player_sprite(
    ECS::EntityManager &_eM, ECS::Chunks::cPosArr_t &chunks, AssetsLoader &assetsLoader
)
{
    auto player = chunks;
    if (player.empty()) {
        return;
    }
    auto ref = _eM.getEntity(player[0]);
    auto square_player = dynamic_cast<ECS::E::GameEntityRef *>(ref.get());
    if (!square_player) {
        std::cerr << "Failed to cast IEntityRef to GameEntityRef" << std::endl;
        return;
    }
    if (*square_player->getHealth()->get<0>() == 1) {
        square_player->getSprite()->set<0>(assetsLoader.get_asset(P1VD).id);
    } else if (*square_player->getHealth()->get<0>() == 2) {
        square_player->getSprite()->set<0>(assetsLoader.get_asset(P1D).id);
    } else if (*square_player->getHealth()->get<0>() == 3) {
        square_player->getSprite()->set<0>(assetsLoader.get_asset(P1SD).id);
    } else if (*square_player->getHealth()->get<0>() == 4) {
        square_player->getSprite()->set<0>(assetsLoader.get_asset(P1FR).id);
    }
}

void setup_decor(
    ECS::EntityManager &_eM, NetworkManager &networkManager, AssetsLoader &assetsLoader
)
{

    auto background = _eM.createEntities("DecorEntity", 1, ECS::C::ENT_ALIVE);

    for (const auto &entity : background) {
        auto ref = _eM.getEntity(entity);

        auto square_background = dynamic_cast<ECS::E::DecorEntityRef *>(ref.get());
        if (!square_background) {
            std::cerr << "Failed to cast IEntityRef to DecorEntityRef" << std::endl;
            return;
        }
        square_background->getType()->set<0>(GameEntityType::BACKGROUND);
        square_background->getSize()->set<0>(3000);
        square_background->getSize()->set<1>(1080);
        square_background->getSprite()->set<0>(assetsLoader.get_asset(BACKGROUND_PATH).id);
    }

    auto ground = _eM.createEntities("DecorEntity", 250, ECS::C::ENT_ALIVE);

    int i = 0;
    for (const auto &entity : ground) {
        auto ref = _eM.getEntity(entity);

        auto square_ground = dynamic_cast<ECS::E::DecorEntityRef *>(ref.get());
        if (!square_ground) {
            std::cerr << "Failed to cast IEntityRef to DecorEntityRef" << std::endl;
            return;
        }
        square_ground->getType()->set<0>(GameEntityType::WALL);
        square_ground->getSize()->set<0>(80);
        square_ground->getSize()->set<1>(100);
        square_ground->getPosition()->set<0>(i * 80);
        square_ground->getPosition()->set<1>(1080 - 100);
        square_ground->getSprite()->set<0>(assetsLoader.get_asset(FLOOR).id);
        i++;
    }

    auto ceiling = _eM.createEntities("DecorEntity", 250, ECS::C::ENT_ALIVE);

    i = 0;
    for (const auto &entity : ceiling) {
        auto ref = _eM.getEntity(entity);

        auto square_ceiling = dynamic_cast<ECS::E::DecorEntityRef *>(ref.get());
        if (!square_ceiling) {
            std::cerr << "Failed to cast IEntityRef to DecorEntityRef" << std::endl;
            return;
        }
        square_ceiling->getType()->set<0>(GameEntityType::WALL);
        square_ceiling->getSize()->set<0>(80);
        square_ceiling->getSize()->set<1>(100);
        square_ceiling->getPosition()->set<0>(i * 80);
        square_ceiling->getSprite()->set<0>(assetsLoader.get_asset(CEILING).id);
        i++;
    }
}

ECS::Chunks::cPosArr_t setup_player(ECS::EntityManager &_eM, AssetsLoader &assetsLoader)
{
    auto engine = _eM.createEntities("GameAnimatedEntity", 1, ECS::C::ENT_ALIVE);

    for (const auto &entity : engine) {
        auto ref = _eM.getEntity(entity);

        auto *square_engine = dynamic_cast<ECS::E::GameAnimatedEntityRef *>(ref.get());
        if (square_engine == nullptr) {
            std::cerr << "Failed to cast IEntityRef to GameEntityRef" << std::endl;
            return {};
        }
        square_engine->getType()->set<0>(GameEntityType::ENGINE);
        square_engine->getSize()->set<0>(80);
        square_engine->getSize()->set<1>(80);
        square_engine->getRotation()->set<0>(90);
        square_engine->getPosition()->set<0>(1920 / 4);
        square_engine->getPosition()->set<1>(1080 / 2);
        square_engine->getAnimatedSprite()->set<0>(assetsLoader.get_asset(ENGINE_1).id);
        square_engine->getAnimatedSprite()->set<1>(4.0F);
        square_engine->getAnimatedSprite()->set<2>(0);
        square_engine->getAnimatedSprite()->set<3>(8.0F);
        square_engine->getTimer()->set<0>(0.0F);
        square_engine->getNetworkID()->set<0>(0);
    }

    auto player = _eM.createEntities("GameEntity", 1, ECS::C::ENT_ALIVE);

    for (const auto &entity : player) {
        auto ref = _eM.getEntity(entity);

        auto *square_player = dynamic_cast<ECS::E::GameEntityRef *>(ref.get());
        if (square_player == nullptr) {
            std::cerr << "Failed to cast IEntityRef to GameEntityRef" << std::endl;
            return {};
        }
        square_player->getPosition()->set<0>(1920 / 4);
        square_player->getPosition()->set<1>(1080 / 2);
        square_player->getType()->set<0>(GameEntityType::LPLAYER);
        square_player->getColor()->set<1>(255);
        square_player->getColor()->set<3>(255);
        square_player->getCanShoot()->set<0>(0);
        square_player->getCanShoot()->set<1>(0.3F);
        square_player->getSize()->set<0>(80);
        square_player->getSize()->set<1>(80);
        square_player->getRotation()->set<0>(90);
        square_player->getSprite()->set<0>(assetsLoader.get_asset(P1FR).id);
        square_player->getHealth()->set<0>(4);
        square_player->getNetworkID()->set<0>(0);
    }
    return player;
}

int main(int ac, char **av)
{
    Camera2D camera = {};
    init_camera(camera);

    srand(time(NULL));
    AssetsLoader assetsLoader;
    ECS::EntityManager _eM(FIXED_TIMESTEP);
    ECS::S::MoveOtherPlayerSystem moveOtherPlayerSystem;
    ECS::S::DestroyEntitiesSystem destroyEntitiesSystem(_eM);
    net::RTypeClient client(
        _eM, moveOtherPlayerSystem.playerStates, destroyEntitiesSystem.entitiesDestroyed,
        camera.target.x
    );
    std::uint16_t port = 0;
    if (ac != 3) {
        std::cerr << "Usage: ./rtype_client ip port" << std::endl;
        return 1;
    }
    port = std::stoi(av[2]);

    std::cout << "Connecting to server on ip " << av[1] << ":" << port << std::endl;
    while (!client.connect_tcp(av[1], port)) {
        std::cerr << "Failed to connect to server, retrying in 1 second" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    client.connect_udp(av[1], port);

    InitWindow(1920, 1080, "R-Type");
    SetTargetFPS(60);

    assetsLoader.load_assets(paths);

    client.ennemySpriteId = assetsLoader.get_asset(E1FC).id;
    client.bulletSpriteId = assetsLoader.get_asset(BASE_BULLET_PATH).id;

    NetworkManager networkManager;

    // Engine modules
    engine::module::Render mRender(camera, assetsLoader);
    mRender.load(_eM);
    engine::module::Spatial2D mSpatial2D;
    mSpatial2D.load(_eM);

    // Rtype systems
    ECS::S::ShowInfoSystem showInfoSystem(camera);
    ECS::S::UpdateEnginePosition updateEnginePosition;
    ECS::S::MovePlayerSystem moveSystem(client);
    ECS::S::MoveBackgroundSystem moveBackgroundSystem;
    ECS::S::MoveEnnemySystem moveEnnemySystem;
    ECS::S::GetPlayerPositionSystem getPlayerPositionSystem;

    // System nodes
    ECS::S::SystemTreeNode rTypeFixedNode(
        "RtypeFixedNode",
        {&moveBackgroundSystem, &moveEnnemySystem, &updateEnginePosition, &moveSystem}
    );
    _eM.registerFixedSystemNode(rTypeFixedNode, ROOT_SYS_GROUP, false, true);
    ECS::S::SystemTreeNode rTypeNode(
        "RtypeNode",
        {&moveOtherPlayerSystem, &destroyEntitiesSystem, &getPlayerPositionSystem, &showInfoSystem}
    );
    _eM.registerSystemNode(rTypeNode, ROOT_SYS_GROUP, false, true);

    // Entity pools
    ECS::E::GameAnimatedEntityPool gameAnimatedEntityPool;
    _eM.registerEntityPool(&gameAnimatedEntityPool);
    ECS::E::GameEntityPool gameEntityPool;
    _eM.registerEntityPool(&gameEntityPool);
    ECS::E::DecorAnimatedEntityPool decorAnimatedEntityPool;
    _eM.registerEntityPool(&decorAnimatedEntityPool);
    ECS::E::DecorEntityPool decorEntityPool;
    _eM.registerEntityPool(&decorEntityPool);



    setup_decor(_eM, networkManager, assetsLoader);

    auto player = setup_player(_eM, assetsLoader);
    client.playerPos = player[0];

    Vector2 playerPosition = {0, 0};
    char playerAlive = 1;


    auto curr_time = std::chrono::steady_clock::now();

    size_t frame = 0;

    while (!WindowShouldClose()) {
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        moveSystem.frame = frame;
        auto new_time = std::chrono::steady_clock::now();
        auto dt = std::chrono::duration<float>(new_time - curr_time).count();
        curr_time = new_time;


        if (client.started)
            update_camera(camera, dt);
        moveBackgroundSystem.cameraX = camera.target.x;
        update_player_sprite(_eM, player, assetsLoader);

        playerPosition = get_player_position(_eM, player);
        moveEnnemySystem.playersPos = getPlayerPositionSystem.playersPos;
        getPlayerPositionSystem.playersPos.clear();
        updateEnginePosition.playerPosition = playerPosition;

        playerAlive = player_is_alive(_eM, player);
        updateEnginePosition.playerAlive = playerAlive;

        showInfoSystem.one_time = false;
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            if (client.started) {
                if (_eM.addTime(dt)) {
                    frame++;
                }
            }
            EndMode2D();
        }
        EndDrawing();

        // create palyer for each player states left over
        for (auto &playerState : moveOtherPlayerSystem.playerStates) {
            std::cout << "Creating player for player state and netid: " << playerState.netId
                      << std::endl;
            auto engine = _eM.createEntities("GameAnimatedEntity", 1, ECS::C::ENT_ALIVE);
            for (const auto &entity : engine) {
                auto ref = _eM.getEntity(entity);

                auto *square_engine = dynamic_cast<ECS::E::GameAnimatedEntityRef *>(ref.get());
                if (square_engine == nullptr) {
                    std::cerr << "Failed to cast IEntityRef to GameEntityRef" << std::endl;
                    return 1;
                }
                square_engine->getType()->set<0>(GameEntityType::ENGINE);
                square_engine->getSize()->set<0>(80);
                square_engine->getSize()->set<1>(80);
                square_engine->getRotation()->set<0>(90);
                square_engine->getPosition()->set<0>(playerState.x);
                square_engine->getPosition()->set<1>(playerState.y);
                square_engine->getAnimatedSprite()->set<0>(assetsLoader.get_asset(ENGINE_1).id);
                square_engine->getAnimatedSprite()->set<1>(4.0F);
                square_engine->getAnimatedSprite()->set<2>(0);
                square_engine->getAnimatedSprite()->set<3>(8.0F);
                square_engine->getTimer()->set<0>(0.0F);
                square_engine->getNetworkID()->set<0>(playerState.netId);
            }
            auto player = _eM.createEntities("GameEntity", 1, ECS::C::ENT_ALIVE);
            for (const auto &entity : player) {
                auto ref = _eM.getEntity(entity);

                auto *square_player = dynamic_cast<ECS::E::GameEntityRef *>(ref.get());
                if (square_player == nullptr) {
                    std::cerr << "Failed to cast IEntityRef to GameEntityRef" << std::endl;
                    return 1;
                }
                square_player->getPosition()->set<0>(playerState.x * 300.0F);
                square_player->getPosition()->set<1>(playerState.y * 300.0F);
                square_player->getVelocity()->set<0>(playerState.vx * 300.0F);
                square_player->getVelocity()->set<1>(playerState.vy * 300.0F);
                square_player->getType()->set<0>(GameEntityType::PLAYER);
                square_player->getColor()->set<1>(255);
                square_player->getColor()->set<3>(255);
                square_player->getCanShoot()->set<0>(true);
                square_player->getCanShoot()->set<1>(0.3F);
                square_player->getSize()->set<0>(80);
                square_player->getSize()->set<1>(80);
                square_player->getRotation()->set<0>(90);
                square_player->getSprite()->set<0>(assetsLoader.get_asset(P1FR).id);
                square_player->getHealth()->set<0>(4);
                square_player->getNetworkID()->set<0>(playerState.netId);
            }
        }
        moveOtherPlayerSystem.playerStates.clear();
        destroyEntitiesSystem.entitiesDestroyed.clear();
    }
    CloseWindow();
    return 0;
}
