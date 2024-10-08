#include <cstdlib>
#ifndef EXAMPLE
#include <chrono>
#include <thread>

#include <raylib.h>
#include "AssetsPath.hpp"

// ECS includes
#include "lib_ecs/Components/PureComponentPools.hpp"
#include "Square.hpp"
#include "Systems.hpp"
#include "lib_ecs/EntityManager.hpp"
#include "lib_ecs/Systems/Query.hpp"
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
    static float camera_cooldown = 0.01;

    if (camera_cooldown > 0) {
        camera_cooldown -= dt;
        return;
    }

    camera_cooldown = 0.01;
    Vector2 old = camera.target;
    camera.target = {old.x + 3, 1080 / 2};
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

    ECS::S::DrawSystem drawSystem;
    ECS::S::MovePlayerSystem moveSystem;
    ECS::S::ApplyVelocitySystem applyVelocitySystem;
    ECS::S::SpawnEnnemySystem spawnEnnemySystem(_eM, assetsLoader, camera);
    ECS::S::ShootSystem shootSystem(_eM, assetsLoader);
    ECS::S::DrawSpriteSystem drawSpriteSystem(assetsLoader, camera);
    ECS::S::MoveBackgroundSystem moveBackgroundSystem(camera);

    ECS::E::SquarePool squarePool;

    ECS::S::SystemTreeNode demoNode(
        42, {&spawnEnnemySystem},
        {&moveBackgroundSystem, &moveSystem, &applyVelocitySystem, &shootSystem, &drawSystem,
         &drawSpriteSystem}
    );

    _eM.registerSystemNode(demoNode, ECS::S::ROOTSYSGROUP, false, true);

    _eM.registerEntityPool(&squarePool);

    auto background = _eM.createEntities("Square", 1, ECS::C::ENT_ALIVE);

    for (const auto &entity : background) {
        auto ref = _eM.getEntity(entity);

        auto square_background = dynamic_cast<ECS::E::SquareRef *>(ref.get());
        if (!square_background) {
            std::cerr << "Failed to cast IEntityRef to SquareRef" << std::endl;
            return -1;
        }
        square_background->getType()->set<0>(SquareType::BACKGROUND);
        square_background->getColor()->set<3>(255);
        square_background->getSize()->set<0>(3000);
        square_background->getSize()->set<1>(1080);
        square_background->getSprite()->set<0>(assetsLoader.get_asset(BACKGROUND_PATH).id);
    }

    auto ground = _eM.createEntities("Square", 250, ECS::C::ENT_ALIVE);

    int i = 0;
    for (const auto &entity : ground) {
        auto ref = _eM.getEntity(entity);

        auto square_ground = dynamic_cast<ECS::E::SquareRef *>(ref.get());
        if (!square_ground) {
            std::cerr << "Failed to cast IEntityRef to SquareRef" << std::endl;
            return -1;
        }
        square_ground->getType()->set<0>(SquareType::WALL);
        square_ground->getColor()->set<2>(255);
        square_ground->getColor()->set<3>(255);
        square_ground->getSize()->set<0>(80);
        square_ground->getSize()->set<1>(100);
        square_ground->getPosition()->set<0>(i * 80);
        square_ground->getPosition()->set<1>(1080 - 100);
        square_ground->getSprite()->set<0>(assetsLoader.get_asset(FLOOR).id);
        i++;
    }

    auto ceiling = _eM.createEntities("Square", 250, ECS::C::ENT_ALIVE);

    i = 0;
    for (const auto &entity : ceiling) {
        auto ref = _eM.getEntity(entity);

        auto square_ceiling = dynamic_cast<ECS::E::SquareRef *>(ref.get());
        if (!square_ceiling) {
            std::cerr << "Failed to cast IEntityRef to SquareRef" << std::endl;
            return -1;
        }
        square_ceiling->getType()->set<0>(SquareType::WALL);
        square_ceiling->getColor()->set<2>(255);
        square_ceiling->getColor()->set<3>(255);
        square_ceiling->getSize()->set<0>(80);
        square_ceiling->getSize()->set<1>(100);
        square_ceiling->getPosition()->set<0>(i * 80);
        square_ceiling->getSprite()->set<0>(assetsLoader.get_asset(CEILING).id);
        i++;
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
    }

    auto curr_time = std::chrono::steady_clock::now();
    while (!WindowShouldClose()) {
        auto new_time = std::chrono::steady_clock::now();
        auto dt = std::chrono::duration<float>(new_time - curr_time).count();
        curr_time = new_time;

        update_camera(camera, dt);
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            applyVelocitySystem.deltaTime = dt;
            shootSystem.deltaTime = dt;
            BeginMode2D(camera);
            _eM.runSystems();
            EndMode2D();
        }
        EndDrawing();
        // sleep for 16ms
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
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