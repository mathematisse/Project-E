#include "MainMenu.hpp"
#include "AssetsPath.hpp"
#include "raygui.h"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

bool MainMenu::is_a_valid_port(const std::string &port)
{
    if (port.empty()) {
        return false;
    }
    for (const auto &c : port) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}

void MainMenu::try_to_connect(void)
{
    int port = std::stoi(_port);

    if (!_client.connect_tcp(_ip, port)) {
        return;
    }
    _client.connect_udp(_ip, port);
    _connected = true;
}

void MainMenu::show_info_box(void)
{
    static const std::string ip_text = "on ip " + _ip + ":" + _port;
    static const std::string message = "Failed to connect to server ";

    if (_showInfoBox) {
        GuiWindowBox((Rectangle){ WINDOW_WIDTH / 2 - 300, WINDOW_HEIGHT / 2 - 100, 600, 250 }, "Info");
        GuiLabel((Rectangle){ WINDOW_WIDTH / 2 - 175, WINDOW_HEIGHT / 2 - 50, 600, 20 }, message.c_str());
        GuiLabel((Rectangle){ WINDOW_WIDTH / 2 - 175, WINDOW_HEIGHT / 2, 350, 20 }, ip_text.c_str());

        if (GuiButton((Rectangle){ WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2 + 50, 100, 30 }, "OK")) {
            _showInfoBox = false;
        }
    }
}

void MainMenu::get_ip_and_port(void)
{
    static bool IPBoxEditMode = false;
    static bool portBoxEditMode = false;
    static char inputIP[64] = "";
    static char inputPort[64] = "";

    DrawRectangle(150, 500, 400, 200, Fade(WHITE, 0.8f));
    GuiLabel((Rectangle){ 200, 550, 200, 30 }, "IP:");
    GuiLabel((Rectangle){ 200, 600, 200, 30 }, "Port:");
    GuiTextBox((Rectangle){ 300, 550, 200, 30 }, inputIP, 64, IPBoxEditMode);
    GuiTextBox((Rectangle){ 300, 600, 200, 30 }, inputPort, 64, portBoxEditMode);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){ 300, 550, 200, 30 })) {
            IPBoxEditMode = !IPBoxEditMode;
        } else {
            IPBoxEditMode = false;
            if (!((std::string)inputIP).empty())
                _ip = inputIP;
        }
        if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){ 300, 600, 200, 30 })) {
            portBoxEditMode = !portBoxEditMode;
        } else {
            portBoxEditMode = false; 
            if (is_a_valid_port(inputPort))
                _port = inputPort;
        }
    }
}

MainMenu::MainMenu(net::RTypeClient &client, AssetsLoader &assetsLoader) : _client(client)
{
    _background = assetsLoader.get_asset(MENU_BACKGROUND);
}

bool MainMenu::open(void)
{
    GuiSetStyle(DEFAULT, TEXT_SIZE, 30);

    while (!WindowShouldClose() && (!_connected || _showInfoBox))
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawTexture(_background, 0, 0, WHITE);
            DrawText("R-Type", WINDOW_WIDTH / 2 - 160, WINDOW_HEIGHT / 2 - 300, 80, RAYWHITE);

            get_ip_and_port();

            if (GuiButton((Rectangle){ WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 + 200, 300, 100 }, "Start")) {
                try_to_connect();
                if (!_connected) {
                    _showInfoBox = true;
                }
            }
            show_info_box();

        EndDrawing();
    }
    if (!_connected) {
        CloseWindow();
        return (false);
    }
    return (true);
}
