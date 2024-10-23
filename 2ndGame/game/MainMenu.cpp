#include "MainMenu.hpp"
#include "AssetsPath.hpp"
#include <chrono>
#include "raylib.h"
#include "raygui.h"

void MainMenu::show_settings(void)
{
    static int old_volume = 50;

    if (_showSettings) {
        GuiWindowBox(
            (Rectangle) {WINDOW_WIDTH / 2 - 400, WINDOW_HEIGHT / 2 - 100, 800, 500}, "Settings"
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
            "Color Blind:"
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

MainMenu::MainMenu(AssetsLoader &assetsLoader)
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

    while (!WindowShouldClose() && (!_started || _showSettings)) {
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
        DrawText("Game", WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 300, 80, RAYWHITE);
        if (settings.color_blind || settings.color_blind_simulation) {
            EndShaderMode();
        }

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
            !_showSettings) {
            _started = true;
        }
        show_settings();

        EndDrawing();
    }
    StopMusicStream(_music);
    if (!_started) {
        CloseWindow();
        CloseAudioDevice();
        return (false);
    }
    return (true);
}
