#pragma once

#include "raylib.h"
#include "render/AssetsLoader.hpp"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

class MainMenu {
    struct Settings {
        bool color_blind = false;
        bool color_blind_simulation = false;
        float volume = 50;
    };

public:
    MainMenu(AssetsLoader &assetsLoader);
    ~MainMenu();

    bool open(void);
    Settings settings;
    Shader colorblindnessShader;
    Shader colorblindSimShader;

private:
    void show_settings(void);

    bool _started = false;
    bool _showSettings = false;
    Texture2D _background;
    Music _music;
};
