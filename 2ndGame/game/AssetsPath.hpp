#include <string>
#include <vector>

#define MENU_BACKGROUND                            \
    {                                              \
        "assets", "other_game", "fantasy_life.png" \
    }

#define GAME_BACKGROUND                          \
    {                                            \
        "assets", "other_game", "background.png" \
    }

#define MENU_MUSIC                                               \
    {                                                            \
        "assets", "other_game", "musics", "fantasy_life_ost.mp3" \
    }

#define GAME_MUSIC                                               \
    {                                                            \
        "assets", "other_game", "musics", "kingdom_rush_ost.mp3" \
    }

#define VISIBILITY_SHADER                                        \
    {                                                            \
        "assets", "other_game", "shader", "high_visibility.frag" \
    }

#define COLORBLIND_SHADER                                   \
    {                                                       \
        "assets", "other_game", "shader", "colorblind.frag" \
    }

#define EMPTY_TOWER                               \
    {                                             \
        "assets", "other_game", "empty_tower.png" \
    }

const std::vector<std::vector<std::string>> paths = {
    MENU_BACKGROUND,
    GAME_BACKGROUND,
    EMPTY_TOWER,
};