#ifndef EXAMPLE
#include <chrono>
#include <thread>

#include <raylib.h>
#include "AssetsLoader.hpp"

// ECS includes
#include "lib_ecs/Components/PureComponentPools.hpp"
#include "Square.hpp"
#include "Systems.hpp"
#include "lib_ecs/EntityManager.hpp"
#include "lib_ecs/Systems/Query.hpp"
#include "lib_ecs/Systems/SystemTree.hpp"

int main()
{
    InitWindow(1920, 1080, "R-Type");

    SetTargetFPS(60);

    AssetsLoader assetsLoader;
    ECS::EntityManager _eM;

    ECS::S::DrawSystem drawSystem;
    ECS::S::MovePlayerSystem moveSystem;
    ECS::S::ApplyVelocitySystem applyVelocitySystem;

    ECS::E::SquarePool squarePool;

    ECS::S::SystemTreeNode demoNode(42, {&applyVelocitySystem, &moveSystem}, {&drawSystem});

    _eM.registerSystemNode(demoNode, ECS::S::ROOTSYSGROUP, false, true);

    _eM.registerEntityPool(&squarePool);

    auto player = _eM.createEntities("Square", 1, ECS::C::ENT_ALIVE);

    for (auto &entity : player) {
        auto ref = _eM.getEntity(entity);
        
        auto square_player = dynamic_cast<ECS::E::SquareRef*>(ref.get());
        if (!square_player) {
            std::cerr << "Failed to cast IEntityRef to SquareRef" << std::endl;
            return -1;
        }
        square_player->setVelocity(ECS::C::VelocityRef(0.0f, 0.0f, 200.0f));
        square_player->setType(ECS::C::TypeRef(SquareType::PLAYER));
    }

    auto curr_time = std::chrono::steady_clock::now();
    while (!WindowShouldClose()) {
        auto new_time = std::chrono::steady_clock::now();
        auto dt = std::chrono::duration<float>(new_time - curr_time).count();
        curr_time = new_time;

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            applyVelocitySystem.deltaTime = dt;
            _eM.runSystems();
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