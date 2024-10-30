#pragma once

#include <string>
#include <vector>

#define BASE_BULLET_PATH                                               \
    {                                                                  \
        "assets", "Mainshipweapons", "Aseprite", "Cannon_bullet.png" \
    }
#define CUT_BULLET_PATH                                                 \
    {                                                                   \
        "assets", "Mainshipweapons", "Aseprite", "Cannon_bulletC.png" \
    }
#define BIG_SHOT_PATH                                             \
    {                                                             \
        "assets", "Mainshipweapons", "Aseprite", "big_shot.png" \
    }
#define POWER_UP_PATH                                                            \
    {                                                                            \
        "assets", "MainShip", "MainShipWeapons", "Aseprite", "power_up.png" \
    }
#define P1F                                                     \
    {                                                           \
        "assets", "MainShip", "MainShipBases", "Aseprite", \
            "MainShipBaseFull health.png"                \
    }
#define P1FR                                                                           \
    {                                                                                  \
        "assets", "MainShip", "MainShipBases", "Aseprite", "MainShipFResized.png" \
    }
#define P2                                                                        \
    {                                                                             \
        "assets", "MainShip", "MainShipBases", "Aseprite", "OtherPlayer.png" \
    }
#define P1SD                                                               \
    {                                                                      \
        "assets", "MainShip", "MainShipBases", "Aseprite", "P1SD.png" \
    }
#define P1D                                                               \
    {                                                                     \
        "assets", "MainShip", "MainShipBases", "Aseprite", "P1D.png" \
    }
#define P1VD                                                               \
    {                                                                      \
        "assets", "MainShip", "MainShipBases", "Aseprite", "P1VD.png" \
    }
#define E1F                                                        \
    {                                                              \
        "assets", "Klaed", "Weapons", "Aseprite", "FigtherT1.png" \
    }
#define E1FC                                                          \
    {                                                                 \
        "assets", "Klaed", "Weapons", "Aseprite", "00_FigtherT1.png" \
    }
#define BACKGROUND_PATH                  \
    {                                    \
        "assets", "SpaceBackground.png" \
    }
#define MENU_BACKGROUND                 \
    {                                   \
        "assets", "menu_background.png" \
    }
#define LOADING_BACKGROUND                 \
    {                                      \
        "assets", "loading_background.png" \
    }
#define FLOOR                 \
    {                         \
        "assets", "floor.png" \
    }
#define CEILING                 \
    {                           \
        "assets", "ceiling.png" \
    }
#define ENGINE_1                                                                       \
    {                                                                                  \
        "assets", "MainShip", "MainShipEngineEffects", "Aseprite", "Engine1.png" \
    }
#define FRIGATE_1                                                \
    {                                                            \
        "assets", "Klaed", "Weapons", "Aseprite", "frigate.png" \
    }

#define MENU_MUSIC                              \
    {                                           \
        "assets", "musics", "RetroSaxGuy.ogg" \
    }

#define LOADING_MUSIC                           \
    {                                           \
        "assets", "musics", "WaitingMusic.ogg" \
    }

#define GAME_MUSIC                                  \
    {                                               \
        "assets", "musics", "Shovel_Knight_OST.ogg" \
    }

#define VISIBILITY_SHADER                          \
    {                                              \
        "assets", "shader", "high_visibility.frag" \
    }

#define COLORBLIND_SHADER                     \
    {                                         \
        "assets", "shader", "colorblind.frag" \
    }

const std::vector<std::vector<std::string>> paths = {
    BASE_BULLET_PATH,
    P1F,
    P1FR,
    P1SD,
    P1D,
    P1VD,
    E1F,
    BACKGROUND_PATH,
    FLOOR,
    CEILING,
    E1FC,
    CUT_BULLET_PATH,
    ENGINE_1,
    BIG_SHOT_PATH,
    FRIGATE_1,
    MENU_BACKGROUND,
    LOADING_BACKGROUND,
    P2,
    POWER_UP_PATH
};