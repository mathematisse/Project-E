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

#define P1F                                                                                          \
    {                                                                                                \
        "assets", "Main Ship", "Main Ship - Bases", "Aseprite", "Main Ship - Base - Full health.png" \
    }
#define P1FR                                                                           \
    {                                                                                  \
        "assets", "Main Ship", "Main Ship - Bases", "Aseprite", "MainShipFResized.png" \
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

const std::vector<std::vector<std::string>> paths = {BASE_BULLET_PATH, P1F,     P1FR,    E1F,
                                                     BACKGROUND_PATH,  FLOOR,   CEILING, E1FC,
                                                     CUT_BULLET_PATH,  ENGINE_1};