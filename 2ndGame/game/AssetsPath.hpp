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

#define ARCHER1_TOWER                               \
    {                                               \
        "assets", "other_game", "archer_tower1.png" \
    }
#define ARCHER2_TOWER                               \
    {                                               \
        "assets", "other_game", "archer_tower2.png" \
    }
#define ARCHER3_TOWER                               \
    {                                               \
        "assets", "other_game", "archer_tower3.png" \
    }

#define WIZARD1_TOWER                               \
    {                                               \
        "assets", "other_game", "wizard_tower1.png" \
    }
#define WIZARD2_TOWER                               \
    {                                               \
        "assets", "other_game", "wizard_tower2.png" \
    }
#define WIZARD3_TOWER                               \
    {                                               \
        "assets", "other_game", "wizard_tower3.png" \
    }
#define GOBLIN                               \
    {                                        \
        "assets", "other_game", "goblin.png" \
    }

#define ARROW                               \
    {                                       \
        "assets", "other_game", "arrow.png" \
    }

#define FIREBALL                               \
    {                                          \
        "assets", "other_game", "fireball.png" \
    }

const std::vector<std::vector<std::string>> paths = {
    MENU_BACKGROUND, GAME_BACKGROUND, EMPTY_TOWER,   ARCHER1_TOWER, ARCHER2_TOWER, ARCHER3_TOWER,
    WIZARD1_TOWER,   WIZARD2_TOWER,   WIZARD3_TOWER, GOBLIN,        ARROW,         FIREBALL
};