#pragma once

#include "raylib.h"
#include <string>
#include <vector>
#include <iostream>
#include "AssetsLoader.hpp"
#include "RTypeClient.hpp"

class MainMenu {
public:
    MainMenu(net::RTypeClient &client, AssetsLoader &assetsLoader);
    ~MainMenu() = default;

    bool open(void);

private:
    void get_ip_and_port(void);
    void show_info_box(void);
    void try_to_connect(void);
    bool is_a_valid_port(const std::string &port);

    std::string _ip = "0.0.0.0";
    std::string _port = "4242";
    bool _connected = false;
    bool _showInfoBox = false;
    Texture2D _background;
    net::RTypeClient &_client;
};
