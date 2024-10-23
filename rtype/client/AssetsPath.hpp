#include <string>
#include <vector>

#define BASE_BULLET_PATH                                               \
    {                                                                  \
        "assets", "Main ship weapons", "Aseprite", "Cannon_bullet.png" \
    }
#define CUT_BULLET_PATH                                                 \
    {                                                                   \
        "assets", "Main ship weapons", "Aseprite", "Cannon_bulletC.png" \
    }
#define BIG_SHOT_PATH                                             \
    {                                                             \
        "assets", "Main ship weapons", "Aseprite", "big_shot.png" \
    }

#define P1F                                                     \
    {                                                           \
        "assets", "Main Ship", "Main Ship - Bases", "Aseprite", \
            "Main Ship - Base - Full health.png"                \
    }
#define P1FR                                                                           \
    {                                                                                  \
        "assets", "Main Ship", "Main Ship - Bases", "Aseprite", "MainShipFResized.png" \
    }
#define P2                                                                        \
    {                                                                             \
        "assets", "Main Ship", "Main Ship - Bases", "Aseprite", "OtherPlayer.png" \
    }
#define P1SD                                                               \
    {                                                                      \
        "assets", "Main Ship", "Main Ship - Bases", "Aseprite", "P1SD.png" \
    }
#define P1D                                                               \
    {                                                                     \
        "assets", "Main Ship", "Main Ship - Bases", "Aseprite", "P1D.png" \
    }
#define P1VD                                                               \
    {                                                                      \
        "assets", "Main Ship", "Main Ship - Bases", "Aseprite", "P1VD.png" \
    }
#define E1F                                                        \
    {                                                              \
        "assets", "Kla'ed", "Weapons", "Aseprite", "FigtherT1.png" \
    }
#define E1FC                                                          \
    {                                                                 \
        "assets", "Kla'ed", "Weapons", "Aseprite", "00_FigtherT1.png" \
    }
#define BACKGROUND_PATH                  \
    {                                    \
        "assets", "Space Background.png" \
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
        "assets", "Main Ship", "Main Ship - Engine Effects", "Aseprite", "Engine1.png" \
    }
#define FRIGATE_1                                                \
    {                                                            \
        "assets", "Kla'ed", "Weapons", "Aseprite", "frigate.png" \
    }

#define MENU_MUSIC                              \
    {                                           \
        "assets", "musics", "Retro-Sax-Guy.ogg" \
    }

#define LOADING_MUSIC                           \
    {                                           \
        "assets", "musics", "Waiting-Music.ogg" \
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
    P2
};