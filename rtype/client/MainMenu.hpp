#pragma once

#include "raylib.h"
#include <string>
#include "render/AssetsLoader.hpp"
#include "RTypeClient.hpp"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

class MainMenu {
    struct Settings {
        bool auto_shoot = false;
        bool color_blind = false;
        bool color_blind_simulation = false;
        float volume = 50;
    };

public:
    MainMenu(net::RTypeClient &client, AssetsLoader &assetsLoader);
    ~MainMenu();

    bool open(void);
    Settings settings;
    Shader colorblindnessShader;
    Shader colorblindSimShader;

private:
    void get_ip_and_port(void);
    void show_info_box(void);
    void show_settings(void);
    void try_to_connect(void);
    bool is_a_valid_port(const std::string &port);

    std::string _ip = "0.0.0.0";
    std::string _port = "4242";
    bool _connected = false;
    bool _showInfoBox = false;
    bool _showSettings = false;
    Texture2D _background;
    Music _music;
    net::RTypeClient &_client;
};
