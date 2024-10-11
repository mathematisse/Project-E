#include "DecorSquare.hpp"
#include <cstdlib>
#ifndef EXAMPLE
#include <chrono>

#include <raylib.h>
#include "AssetsPath.hpp"

// ECS includes
#include "lib_ecs/Components/PureComponentPools.hpp"
#include "Square.hpp"
#include "Systems.hpp"
#include "lib_ecs/EntityManager.hpp"
#include "lib_ecs/Systems/SystemTree.hpp"

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

int main()
{
    InitWindow(1920, 1080, "R-Type");

    Camera2D camera = {};
    init_camera(camera);

    SetTargetFPS(60);
    srand(time(NULL));

    AssetsLoader assetsLoader;
    assetsLoader.load_assets(paths);

    ECS::EntityManager _eM;

    ECS::S::DrawSystem drawSystem(camera);
    ECS::S::MovePlayerSystem moveSystem;
    ECS::S::ApplyVelocitySystem applyVelocitySystem;
    ECS::S::SpawnEnnemySystem spawnEnnemySystem(_eM, assetsLoader, camera);
    ECS::S::ShootSystem shootSystem(_eM, assetsLoader);
    ECS::S::DrawSpriteSystem drawSpriteSystem(assetsLoader, camera);
    ECS::S::MoveBackgroundSystem moveBackgroundSystem(camera);
    ECS::S::MoveEnnemySystem moveEnnemySystem;
    ECS::S::ColliderSystem colliderSystem;
    ECS::S::CountEnnemyAliveSystem countEnnemyAliveSystem(spawnEnnemySystem.ennemyCount);
    ECS::S::ShowInfoSystem showInfoSystem(camera);
    ECS::S::ClockSystem clockSystem(assetsLoader);
    ECS::S::UpdateEnginePosition updateEnginePosition;

    ECS::E::SquarePool squarePool;
    ECS::E::DecorSquarePool decorSquarePool;

    ECS::S::SystemTreeNode demoNode(
        42, {&spawnEnnemySystem, &countEnnemyAliveSystem},
        {&moveBackgroundSystem, &moveEnnemySystem, &moveSystem, &applyVelocitySystem, &updateEnginePosition,
         &shootSystem, &colliderSystem, &drawSpriteSystem, &drawSystem, &showInfoSystem, &clockSystem}
    );

    _eM.registerSystemNode(demoNode, ECS::S::ROOTSYSGROUP, false, true);

    _eM.registerEntityPool(&decorSquarePool);
    _eM.registerEntityPool(&squarePool);

    auto background = _eM.createEntities("DecorSquare", 1, ECS::C::ENT_ALIVE);

    for (const auto &entity : background) {
        auto ref = _eM.getEntity(entity);

        auto square_background = dynamic_cast<ECS::E::DecorSquareRef *>(ref.get());
        if (!square_background) {
            std::cerr << "Failed to cast IEntityRef to DecorSquareRef" << std::endl;
            return -1;
        }
        square_background->getType()->set<0>(SquareType::BACKGROUND);
        square_background->getSize()->set<0>(3000);
        square_background->getSize()->set<1>(1080);
        square_background->getSprite()->set<0>(assetsLoader.get_asset(BACKGROUND_PATH).id);
    }

    auto ground = _eM.createEntities("DecorSquare", 250, ECS::C::ENT_ALIVE);

    int i = 0;
    for (const auto &entity : ground) {
        auto ref = _eM.getEntity(entity);

        auto square_ground = dynamic_cast<ECS::E::DecorSquareRef *>(ref.get());
        if (!square_ground) {
            std::cerr << "Failed to cast IEntityRef to DecorSquareRef" << std::endl;
            return -1;
        }
        square_ground->getType()->set<0>(SquareType::WALL);
        square_ground->getSize()->set<0>(80);
        square_ground->getSize()->set<1>(100);
        square_ground->getPosition()->set<0>(i * 80);
        square_ground->getPosition()->set<1>(1080 - 100);
        square_ground->getSprite()->set<0>(assetsLoader.get_asset(FLOOR).id);
        i++;
    }

    auto ceiling = _eM.createEntities("DecorSquare", 250, ECS::C::ENT_ALIVE);

    i = 0;
    for (const auto &entity : ceiling) {
        auto ref = _eM.getEntity(entity);

        auto square_ceiling = dynamic_cast<ECS::E::DecorSquareRef *>(ref.get());
        if (!square_ceiling) {
            std::cerr << "Failed to cast IEntityRef to DecorSquareRef" << std::endl;
            return -1;
        }
        square_ceiling->getType()->set<0>(SquareType::WALL);
        square_ceiling->getSize()->set<0>(80);
        square_ceiling->getSize()->set<1>(100);
        square_ceiling->getPosition()->set<0>(i * 80);
        square_ceiling->getSprite()->set<0>(assetsLoader.get_asset(CEILING).id);
        i++;
    }

    auto engine = _eM.createEntities("DecorSquare", 1, ECS::C::ENT_ALIVE);

    for (const auto &entity : engine) {
        auto ref = _eM.getEntity(entity);

        auto square_engine = dynamic_cast<ECS::E::DecorSquareRef *>(ref.get());
        if (!square_engine) {
            std::cerr << "Failed to cast IEntityRef to DecorSquareRef" << std::endl;
            return -1;
        }
        square_engine->getType()->set<0>(SquareType::ENGINE);
        square_engine->getSize()->set<0>(80);
        square_engine->getSize()->set<1>(80);
        square_engine->getSize()->set<2>(90);
        square_engine->getPosition()->set<0>(1920 / 4);
        square_engine->getPosition()->set<1>(1080 / 2);
        square_engine->getSprite()->set<0>(assetsLoader.get_asset(ENGINE_1).id);
        square_engine->getSprite()->set<1>(true);
        square_engine->getSprite()->set<2>(80.0F);
        square_engine->getSprite()->set<3>(80.0F);
        square_engine->getSprite()->set<4>(4.0F);
        square_engine->getSprite()->set<5>(0);
        square_engine->getTimer()->set<0>(0.0F);
        square_engine->getTimer()->set<1>(8.0F);
        std::cout << "Engine created " << square_engine->getSprite()->get<0>() << std::endl;
    }

    auto player = _eM.createEntities("Square", 1, ECS::C::ENT_ALIVE);

    for (const auto &entity : player) {
        auto ref = _eM.getEntity(entity);

        auto square_player = dynamic_cast<ECS::E::SquareRef *>(ref.get());
        if (!square_player) {
            std::cerr << "Failed to cast IEntityRef to SquareRef" << std::endl;
            return -1;
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
        square_player->getSprite()->set<0>(assetsLoader.get_asset(P1FR).id);
        square_player->getHealth()->set<0>(3);
    }

    Vector2 playerPosition = get_player_position(_eM, player);
    char playerAlive = player_is_alive(_eM, player);

    auto curr_time = std::chrono::steady_clock::now();
    while (!WindowShouldClose()) {
        auto new_time = std::chrono::steady_clock::now();
        auto dt = std::chrono::duration<float>(new_time - curr_time).count();
        curr_time = new_time;

        update_camera(camera, dt);
        playerPosition = get_player_position(_eM, player);
        playerAlive = player_is_alive(_eM, player);
        moveEnnemySystem.playerPosition = playerPosition;
        shootSystem.playerPosition = playerPosition;
        applyVelocitySystem.deltaTime = dt;
        shootSystem.deltaTime = dt;
        drawSpriteSystem.deltaTime = dt;
        countEnnemyAliveSystem.ennemyCount = 0;
        showInfoSystem.one_time = false;
        clockSystem.deltaTime = dt;
        updateEnginePosition.playerPosition = playerPosition;
        updateEnginePosition.playerAlive = playerAlive;
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            _eM.runSystems();
            EndMode2D();
        }
        EndDrawing();
        spawnEnnemySystem.ennemyCount = countEnnemyAliveSystem.ennemyCount;
    }
    CloseWindow();
    return 0;
}

#else

#include "network/Client.hpp"
#include <array>
#include <iostream>

void getKeys(std::array<bool, 3> &key)
{
#ifdef _WIN32
    if (GetForegroundWindow() == GetConsoleWindow()) {
        key[0] = GetAsyncKeyState('1') & 0x8000;
        key[1] = GetAsyncKeyState('2') & 0x8000;
        key[2] = GetAsyncKeyState('3') & 0x8000;
    }
#else
    initscr();
    timeout(0); // Non-blocking input
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    int character = getch();
    key[0] = (character == '1');
    key[1] = (character == '2');
    key[2] = (character == '3');

    endwin();
#endif
}

constexpr int PORT = 60000;

int main()
{
    network::CustomClient client;
    client.connect("127.0.0.1", PORT);

    std::array<bool, 3> key = {false, false, false};
    std::array<bool, 3> old_key = {false, false, false};

    bool bQuit = false;
    while (!bQuit) {
        getKeys(key);

        if (key[0] && !old_key[0]) {
            client.PingServer();
        }
        if (key[1] && !old_key[1]) {
            client.MessageAll();
        }
        if (key[2] && !old_key[2]) {
            bQuit = true;
        }
        for (int i = 0; i < 3; i++) {
            std::copy(key.begin(), key.end(), old_key.begin());
        }
        if (client.is_connected()) {
            if (!client.incoming().empty()) {

                auto msg = client.incoming().pop_front().packet;

                switch (msg.header.type) {
                case network::CustomMsgTypes::ServerAccept: {
                    // Server has responded to a ping request
                    std::cout << "Server Accepted Connection\n";
                } break;

                case network::CustomMsgTypes::ServerPing: {
                    // Server has responded to a ping request
                    std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
                    std::chrono::system_clock::time_point timeThen;
                    msg >> timeThen;
                    std::cout << "Ping: " << std::chrono::duration<double>(timeNow - timeThen).count()
                              << "\n";
                } break;

                case network::CustomMsgTypes::ServerMessage: {
                    // Server has responded to a ping request
                    uint32_t clientID = 0;
                    msg >> clientID;
                    std::cout << "Hello from [" << clientID << "]\n";
                } break;
                case network::CustomMsgTypes::ServerDeny:
                case network::CustomMsgTypes::MessageAll:
                    break;
                }
            }
        } else {
            std::cout << "Server Down\n";
            bQuit = true;
        }
    }

    return 0;
}
#endif