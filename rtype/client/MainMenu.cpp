#include "MainMenu.hpp"
#include "AssetsPath.hpp"
#include <chrono>
#include "raylib.h"
#include "raygui.h"

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
        GuiWindowBox(
            (Rectangle) {WINDOW_WIDTH / 2 - 300, WINDOW_HEIGHT / 2 - 100, 600, 250}, "Info"
        );
        GuiLabel(
            (Rectangle) {WINDOW_WIDTH / 2 - 175, WINDOW_HEIGHT / 2 - 50, 600, 20}, message.c_str()
        );
        GuiLabel((Rectangle) {WINDOW_WIDTH / 2 - 175, WINDOW_HEIGHT / 2, 350, 20}, ip_text.c_str());

        if (GuiButton((Rectangle) {WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2 + 50, 100, 30}, "OK")) {
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
    GuiLabel((Rectangle) {200, 550, 200, 30}, "IP:");
    GuiLabel((Rectangle) {200, 600, 200, 30}, "Port:");
    GuiTextBox((Rectangle) {300, 550, 200, 30}, inputIP, 64, IPBoxEditMode);
    GuiTextBox((Rectangle) {300, 600, 200, 30}, inputPort, 64, portBoxEditMode);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(GetMousePosition(), (Rectangle) {300, 550, 200, 30})) {
            IPBoxEditMode = !IPBoxEditMode;
        } else {
            IPBoxEditMode = false;
            if (!((std::string) inputIP).empty())
                _ip = inputIP;
        }
        if (CheckCollisionPointRec(GetMousePosition(), (Rectangle) {300, 600, 200, 30})) {
            portBoxEditMode = !portBoxEditMode;
        } else {
            portBoxEditMode = false;
            if (is_a_valid_port(inputPort))
                _port = inputPort;
        }
    }
}

void MainMenu::show_settings(void)
{
    static int old_volume = 50;

    if (_showSettings) {
        GuiWindowBox(
            (Rectangle) {WINDOW_WIDTH / 2 - 400, WINDOW_HEIGHT / 2 - 100, 800, 500}, "Settings"
        );
        GuiLabel((Rectangle) {WINDOW_WIDTH / 2 - 275, WINDOW_HEIGHT / 2, 350, 20}, "Auto Shoot:");
        GuiCheckBox(
            (Rectangle) {WINDOW_WIDTH / 2 + 100, WINDOW_HEIGHT / 2, 20, 20}, "",
            &settings.auto_shoot
        );
        GuiLabel(
            (Rectangle) {WINDOW_WIDTH / 2 - 275, WINDOW_HEIGHT / 2 + 50, 350, 20},
            "High Visibility:"
        );
        GuiCheckBox(
            (Rectangle) {WINDOW_WIDTH / 2 + 100, WINDOW_HEIGHT / 2 + 50, 20, 20}, "",
            &settings.color_blind
        );
        GuiLabel(
            (Rectangle) {WINDOW_WIDTH / 2 - 275, WINDOW_HEIGHT / 2 + 100, 350, 20},
            "Color Blind Simulation:"
        );
        GuiCheckBox(
            (Rectangle) {WINDOW_WIDTH / 2 + 100, WINDOW_HEIGHT / 2 + 100, 20, 20}, "",
            &settings.color_blind_simulation
        );
        GuiLabel((Rectangle) {WINDOW_WIDTH / 2 - 275, WINDOW_HEIGHT / 2 + 150, 350, 20}, "Volume:");
        GuiSlider(
            (Rectangle) {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 150, 350, 20}, "0", "100",
            &settings.volume, 0, 100
        );
        if (GuiButton(
                (Rectangle) {WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2 + 250, 100, 30}, "Back"
            )) {
            _showSettings = false;
        }

        if ((int) settings.volume != old_volume) {
            old_volume = (int) settings.volume;
            SetMusicVolume(_music, settings.volume / 100.0f);
        }
    }
}

MainMenu::MainMenu(net::RTypeClient &client, AssetsLoader &assetsLoader):
    _client(client)
{
    _background = assetsLoader.get_asset(MENU_BACKGROUND);
    _music = LoadMusicStream(assetsLoader.get_real_path(MENU_MUSIC).c_str());
    PlayMusicStream(_music);
    SetMusicVolume(_music, 50.0f / 100.0f);
    colorblindnessShader = LoadShader(0, assetsLoader.get_real_path(VISIBILITY_SHADER).c_str());
    colorblindSimShader = LoadShader(0, assetsLoader.get_real_path(COLORBLIND_SHADER).c_str());
}

MainMenu::~MainMenu()
{
    UnloadMusicStream(_music);
    UnloadShader(colorblindnessShader);
    UnloadShader(colorblindSimShader);
}

bool MainMenu::open(void)
{
    GuiSetStyle(DEFAULT, TEXT_SIZE, 30);

    while (!WindowShouldClose() && (!_connected || _showInfoBox || _showSettings)) {
        UpdateMusicStream(_music);
        BeginDrawing();
        ClearBackground(RAYWHITE);
        if (settings.color_blind) {
            BeginShaderMode(colorblindnessShader);
        }
        if (settings.color_blind_simulation) {
            BeginShaderMode(colorblindSimShader);
        }
        DrawTexture(_background, 0, 0, WHITE);
        DrawText("R-Type", WINDOW_WIDTH / 2 - 160, WINDOW_HEIGHT / 2 - 300, 80, RAYWHITE);
        if (settings.color_blind || settings.color_blind_simulation) {
            EndShaderMode();
        }

        get_ip_and_port();

        if (GuiButton(
                (Rectangle) {WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 + 150, 300, 100}, "Settings"
            )) {
            _showSettings = true;
        }

        if (GuiButton(
                (Rectangle) {WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 + 300, 300, 100}, "Quit"
            )) {
            EndDrawing();
            break;
        }

        if (GuiButton((Rectangle) {WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2, 300, 100}, "Start") &&
            !_showSettings && !_showInfoBox) {
            try_to_connect();
            if (!_connected) {
                _showInfoBox = true;
            }
        }
        show_info_box();
        show_settings();

        EndDrawing();
    }
    StopMusicStream(_music);
    if (!_connected) {
        CloseWindow();
        CloseAudioDevice();
        return (false);
    }
    return (true);
}
