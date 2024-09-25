#include "network/Client.hpp"
#include <array>
#include <iostream>

#ifdef EXAMPLE
#include "raylib.h"
int main()
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        BeginDrawing();

        if (IsKeyPressed(KEY_ENTER)) {
            // make asan do an error
            std::cout << "Hello, world!" << std::endl;
            int *a = new int[10];
            a[10] = 0;
            delete[] a;
            delete[] a;
        }

        ClearBackground(RAYWHITE);

        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

#else

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