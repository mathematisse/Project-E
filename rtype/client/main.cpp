#include "DecorSquare.hpp"
#include "MainMenu.hpp"
#include <cstdlib>
#include <chrono>
#include <string>

#include <iostream>
#include <raylib.h>
#include "raygui.h"
#include <thread>
#include "AssetsPath.hpp"
#include "MainMenu.hpp"

// ECS includes
#include "lib_ecs/Chunks/ChunkPos.hpp"
#include "lib_ecs/Components/PureComponentPools.hpp"
#include "Square.hpp"
#include "Systems.hpp"
#include "ClientSystems.hpp"
#include "NetSystems.hpp"
#include "DrawSystems.hpp"
#include "lib_ecs/EntityManager.hpp"
#include "lib_ecs/Systems/SystemTree.hpp"
#include "RTypeClient.hpp"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

void init_camera(Camera2D &camera)
{
    camera.target = {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};
    camera.offset = {WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

void update_camera(Camera2D &camera, float dt)
{
    Vector2 old = camera.target;
    camera.target = {old.x + 80 * dt, WINDOW_HEIGHT / 2};
}

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

void update_player_sprite(
    ECS::EntityManager &_eM, ECS::Chunks::cPosArr_t &chunks, AssetsLoader &assetsLoader
)
{
    auto player = chunks;
    if (player.empty()) {
        return;
    }
    auto ref = _eM.getEntity(player[0]);
    auto square_player = dynamic_cast<ECS::E::SquareRef *>(ref.get());
    if (!square_player) {
        std::cerr << "Failed to cast IEntityRef to SquareRef" << std::endl;
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

    auto background = _eM.createEntities("DecorSquare", 1, ECS::C::ENT_ALIVE);

    for (const auto &entity : background) {
        auto ref = _eM.getEntity(entity);

        auto square_background = dynamic_cast<ECS::E::DecorSquareRef *>(ref.get());
        if (!square_background) {
            std::cerr << "Failed to cast IEntityRef to DecorSquareRef" << std::endl;
            return;
        }
        square_background->getType()->set<0>(SquareType::BACKGROUND);
        square_background->getSize()->set<0>(3000);
        square_background->getSize()->set<1>(WINDOW_HEIGHT);
        square_background->getSprite()->set<0>(assetsLoader.get_asset(BACKGROUND_PATH).id);
        square_background->getNetworkID()->set<0>(999999);
    }

    auto ground = _eM.createEntities("DecorSquare", 250, ECS::C::ENT_ALIVE);

    int i = 0;
    for (const auto &entity : ground) {
        auto ref = _eM.getEntity(entity);

        auto square_ground = dynamic_cast<ECS::E::DecorSquareRef *>(ref.get());
        if (!square_ground) {
            std::cerr << "Failed to cast IEntityRef to DecorSquareRef" << std::endl;
            return;
        }
        square_ground->getType()->set<0>(SquareType::WALL);
        square_ground->getSize()->set<0>(80);
        square_ground->getSize()->set<1>(100);
        square_ground->getPosition()->set<0>(i * 80);
        square_ground->getPosition()->set<1>(WINDOW_HEIGHT - 100);
        square_ground->getSprite()->set<0>(assetsLoader.get_asset(FLOOR).id);
        square_ground->getNetworkID()->set<0>(999999);
        i++;
    }

    auto ceiling = _eM.createEntities("DecorSquare", 250, ECS::C::ENT_ALIVE);

    i = 0;
    for (const auto &entity : ceiling) {
        auto ref = _eM.getEntity(entity);

        auto square_ceiling = dynamic_cast<ECS::E::DecorSquareRef *>(ref.get());
        if (!square_ceiling) {
            std::cerr << "Failed to cast IEntityRef to DecorSquareRef" << std::endl;
            return;
        }
        square_ceiling->getType()->set<0>(SquareType::WALL);
        square_ceiling->getSize()->set<0>(80);
        square_ceiling->getSize()->set<1>(100);
        square_ceiling->getPosition()->set<0>(i * 80);
        square_ceiling->getSprite()->set<0>(assetsLoader.get_asset(CEILING).id);
        square_ceiling->getNetworkID()->set<0>(999999);
        i++;
    }
}

ECS::Chunks::cPosArr_t setup_player(ECS::EntityManager &_eM, AssetsLoader &assetsLoader)
{
    auto engine = _eM.createEntities("DecorSquare", 1, ECS::C::ENT_ALIVE);

    for (const auto &entity : engine) {
        auto ref = _eM.getEntity(entity);

        auto *square_engine = dynamic_cast<ECS::E::DecorSquareRef *>(ref.get());
        if (square_engine == nullptr) {
            std::cerr << "Failed to cast IEntityRef to DecorSquareRef" << std::endl;
            return {};
        }
        square_engine->getType()->set<0>(SquareType::ENGINE);
        square_engine->getSize()->set<0>(80);
        square_engine->getSize()->set<1>(80);
        square_engine->getSize()->set<2>(90);
        square_engine->getPosition()->set<0>(WINDOW_WIDTH / 4);
        square_engine->getPosition()->set<1>(WINDOW_HEIGHT / 2);
        square_engine->getSprite()->set<0>(assetsLoader.get_asset(ENGINE_1).id);
        square_engine->getSprite()->set<1>(true);
        square_engine->getSprite()->set<2>(80.0F);
        square_engine->getSprite()->set<3>(80.0F);
        square_engine->getSprite()->set<4>(4.0F);
        square_engine->getSprite()->set<5>(0);
        square_engine->getTimer()->set<0>(0.0F);
        square_engine->getTimer()->set<1>(8.0F);
        square_engine->getNetworkID()->set<0>(0);
    }

    auto player = _eM.createEntities("Square", 1, ECS::C::ENT_ALIVE);

    for (const auto &entity : player) {
        auto ref = _eM.getEntity(entity);

        auto *square_player = dynamic_cast<ECS::E::SquareRef *>(ref.get());
        if (square_player == nullptr) {
            std::cerr << "Failed to cast IEntityRef to SquareRef" << std::endl;
            return {};
        }
        square_player->getPosition()->set<0>(WINDOW_WIDTH / 4);
        square_player->getPosition()->set<1>(WINDOW_HEIGHT / 2);
        square_player->getVelocity()->set<2>(300.0F);
        square_player->getType()->set<0>(SquareType::LPLAYER);
        square_player->getColor()->set<1>(255);
        square_player->getColor()->set<3>(255);
        square_player->getCanShoot()->set<0>(true);
        square_player->getCanShoot()->set<1>(0.3F);
        square_player->getSize()->set<0>(80);
        square_player->getSize()->set<1>(80);
        square_player->getSize()->set<2>(90);
        square_player->getSprite()->set<0>(assetsLoader.get_asset(P1FR).id);
        square_player->getHealth()->set<0>(4);
        square_player->getNetworkID()->set<0>(0);
    }
    return player;
}

int main(int ac, char **av)
{
    if (ac != 1) {
        std::cerr << "Usage: ./rtype_client" << std::endl;
        return 1;
    }

    Camera2D camera = {};
    init_camera(camera);

    srand(time(NULL));
    AssetsLoader assetsLoader;
    ECS::EntityManager _eM;
    ECS::S::MoveOtherPlayerSystem moveOtherPlayerSystem;
    ECS::S::DestroyEntitiesSystem destroyEntitiesSystem(_eM);
    net::RTypeClient client(
        _eM, moveOtherPlayerSystem.playerStates, destroyEntitiesSystem.entitiesDestroyed,
        camera.target.x
    );

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "R-Type");
    SetTargetFPS(60);

    assetsLoader.load_assets(paths);

    client.ennemySpriteId = assetsLoader.get_asset(E1FC).id;
    client.bulletSpriteId = assetsLoader.get_asset(BASE_BULLET_PATH).id;

    MainMenu mainMenu(client, assetsLoader);

    if (!mainMenu.open())
        return 0;
    Texture2D loading_background = assetsLoader.get_asset(LOADING_BACKGROUND);

    NetworkManager networkManager;

    ECS::S::DrawSystem drawSystem(camera);
    ECS::S::ShowInfoSystem showInfoSystem(camera);
    ECS::S::DrawSpriteSystem drawSpriteSystem(assetsLoader, camera);
    ECS::S::AnimationSystem animationSystem(assetsLoader);
    ECS::S::UpdateEnginePosition updateEnginePosition;

    ECS::S::MovePlayerSystem moveSystem(client);
    ECS::S::ApplyVelocitySystem applyVelocitySystem;
    ECS::S::MoveBackgroundSystem moveBackgroundSystem;
    ECS::S::MoveEnnemySystem moveEnnemySystem;
    ECS::S::GetPlayerPositionSystem getPlayerPositionSystem;
    // ECS::S::ColliderSystem colliderSystem;

    // ECS::S::SendDecorStateSystem sendDecorStateSystem;
    // ECS::S::SendSquareStateSystem sendSquareStateSystem;

    // Entity pools
    ECS::E::SquarePool squarePool;
    ECS::E::DecorSquarePool decorSquarePool;

    ECS::S::SystemTreeNode demoFixedNode(
        42,
        {&moveBackgroundSystem, &moveEnnemySystem, &updateEnginePosition, &applyVelocitySystem,
         &moveSystem}
    );

    ECS::S::SystemTreeNode demoNode(
        42,
        {&animationSystem, &moveOtherPlayerSystem, &destroyEntitiesSystem, &getPlayerPositionSystem,
         &drawSpriteSystem, &drawSystem, &showInfoSystem}
    );

    _eM.registerFixedSystemNode(demoFixedNode, ECS::S::ROOTSYSGROUP, false, true);
    _eM.registerSystemNode(demoNode, ECS::S::ROOTSYSGROUP, false, true);

    _eM.registerEntityPool(&decorSquarePool);
    _eM.registerEntityPool(&squarePool);

    setup_decor(_eM, networkManager, assetsLoader);

    auto player = setup_player(_eM, assetsLoader);
    client.playerPos = player[0];

    Vector2 playerPosition = {0, 0};
    char playerAlive = 1;

    applyVelocitySystem.deltaTime = 0.02F;

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
        animationSystem.deltaTime = dt;

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
                EndMode2D();
            } else {
                DrawTexture(loading_background, 0, 0, WHITE);
                DrawText("Waiting for other players...", WINDOW_WIDTH / 2 - 500, WINDOW_HEIGHT / 2, 80, WHITE);
            }
        }
        EndDrawing();

        // create palyer for each player states left over
        for (auto &playerState : moveOtherPlayerSystem.playerStates) {
            std::cout << "Creating player for player state and netid: " << playerState.netId
                      << std::endl;
            auto engine = _eM.createEntities("DecorSquare", 1, ECS::C::ENT_ALIVE);
            for (const auto &entity : engine) {
                auto ref = _eM.getEntity(entity);

                auto *square_engine = dynamic_cast<ECS::E::DecorSquareRef *>(ref.get());
                if (square_engine == nullptr) {
                    std::cerr << "Failed to cast IEntityRef to DecorSquareRef" << std::endl;
                    return 1;
                }
                square_engine->getType()->set<0>(SquareType::ENGINE);
                square_engine->getSize()->set<0>(80);
                square_engine->getSize()->set<1>(80);
                square_engine->getSize()->set<2>(90);
                square_engine->getPosition()->set<0>(playerState.x);
                square_engine->getPosition()->set<1>(playerState.y);
                square_engine->getSprite()->set<0>(assetsLoader.get_asset(ENGINE_1).id);
                square_engine->getSprite()->set<1>(true);
                square_engine->getSprite()->set<2>(80.0F);
                square_engine->getSprite()->set<3>(80.0F);
                square_engine->getSprite()->set<4>(4.0F);
                square_engine->getSprite()->set<5>(0);
                square_engine->getTimer()->set<0>(0.0F);
                square_engine->getTimer()->set<1>(8.0F);
                square_engine->getNetworkID()->set<0>(playerState.netId);
            }
            auto player = _eM.createEntities("Square", 1, ECS::C::ENT_ALIVE);
            for (const auto &entity : player) {
                auto ref = _eM.getEntity(entity);

                auto *square_player = dynamic_cast<ECS::E::SquareRef *>(ref.get());
                if (square_player == nullptr) {
                    std::cerr << "Failed to cast IEntityRef to SquareRef" << std::endl;
                    return 1;
                }
                square_player->getPosition()->set<0>(playerState.x);
                square_player->getPosition()->set<1>(playerState.y);
                square_player->getVelocity()->set<0>(playerState.vx);
                square_player->getVelocity()->set<1>(playerState.vy);
                square_player->getVelocity()->set<2>(300.0F);
                square_player->getType()->set<0>(SquareType::PLAYER);
                square_player->getColor()->set<1>(255);
                square_player->getColor()->set<3>(255);
                square_player->getCanShoot()->set<0>(true);
                square_player->getCanShoot()->set<1>(0.3F);
                square_player->getSize()->set<0>(80);
                square_player->getSize()->set<1>(80);
                square_player->getSize()->set<2>(90);
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
