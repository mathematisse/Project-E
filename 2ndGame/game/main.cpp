#include <iostream>
#include <chrono>
#include <raylib.h>
#include <raygui.h>
#include <vector>
#include <string>
#include "AssetsLoader.hpp"
#include "AssetsPath.hpp"
#include "MainMenu.hpp"

// ECS includes
#include "Tower.hpp"
#include "lib_ecs/Chunks/ChunkPos.hpp"
#include "lib_ecs/Components/PureComponentPools.hpp"
#include "lib_ecs/EntityManager.hpp"
#include "lib_ecs/Systems/SystemTree.hpp"

#include "Decor.hpp"
#include "Systems.hpp"

static const std::vector<std::string> tower_names {"None", "Bomb", "Archer", "Wizard"};

void open_tower_menu(ECS::S::TowerClickSystem &towerClickSystem)
{
    GuiWindowBox((Rectangle) {towerClickSystem.pos.x, towerClickSystem.pos.y, 200, 200}, "Tower");
    GuiLabel(
        (Rectangle) {towerClickSystem.pos.x + 10, towerClickSystem.pos.y + 35, 100, 20}, "Type:"
    );
    GuiLabel(
        (Rectangle) {towerClickSystem.pos.x + 100, towerClickSystem.pos.y + 35, 100, 20},
        tower_names[towerClickSystem.selectedTower.type].c_str()
    );
    GuiLabel(
        (Rectangle) {towerClickSystem.pos.x + 10, towerClickSystem.pos.y + 65, 100, 20}, "Level:"
    );
    GuiLabel(
        (Rectangle) {towerClickSystem.pos.x + 100, towerClickSystem.pos.y + 65, 100, 20},
        std::to_string(towerClickSystem.selectedTower.level).c_str()
    );
    if (GuiButton(
            (Rectangle) {towerClickSystem.pos.x + 25, towerClickSystem.pos.y + 100, 150, 30},
            "Upgrade"
        )) {
        towerClickSystem.selectedTower.level++;
    }
    if (GuiButton(
            (Rectangle) {towerClickSystem.pos.x + 50, towerClickSystem.pos.y + 150, 100, 30}, "Quit"
        )) {
        towerClickSystem.open = false;
    }
}

int main(int ac, char *av[])
{
    if (ac != 1) {
        std::cerr << "Usage: ./game" << std::endl;
        return 1;
    }

    srand(time(NULL));
    AssetsLoader assetsLoader;
    ECS::EntityManager _eM;

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Game");
    InitAudioDevice();
    SetTargetFPS(60);

    assetsLoader.load_assets(paths);
    Music music = LoadMusicStream(assetsLoader.get_real_path(GAME_MUSIC).c_str());

    MainMenu mainMenu(assetsLoader);

    if (!mainMenu.open()) {
        return 0;
    }

    ECS::E::DecorPool decorPool;
    ECS::E::TowerPool towerPool;
    ECS::S::DrawSpriteSystem drawSpriteSystem(assetsLoader);
    ECS::S::TowerClickSystem towerClickSystem;

    ECS::S::SystemTreeNode demoNode(42, {&towerClickSystem, &drawSpriteSystem}, {});

    _eM.registerSystemNode(demoNode, ECS::S::ROOTSYSGROUP, false, true);

    _eM.registerEntityPool(&decorPool);
    _eM.registerEntityPool(&towerPool);

    auto background = _eM.createEntities("Decor", 1, ECS::C::ENT_ALIVE);

    for (const auto &entity : background) {
        auto ref = _eM.getEntity(entity);

        auto square_background = dynamic_cast<ECS::E::DecorRef *>(ref.get());
        if (!square_background) {
            std::cerr << "Failed to cast IEntityRef to DecorRef" << std::endl;
            return 0;
        }
        square_background->getSize()->set<0>(1920);
        square_background->getSize()->set<1>(1080);
        square_background->getSprite()->set<0>(assetsLoader.get_asset(GAME_BACKGROUND).id);
    }

    auto towers = _eM.createEntities("Tower", 9, ECS::C::ENT_ALIVE);

    std::vector<Vector2> towers_positions = {{1287, 224}, {1452, 889}, {616, 266},
                                             {531, 559},  {901, 462},  {1139, 741},
                                             {1012, 271}, {205, 280},  {682, 841}};

    int i = 0;

    for (const auto &entity : towers) {
        auto ref = _eM.getEntity(entity);

        auto square_tower = dynamic_cast<ECS::E::TowerRef *>(ref.get());
        if (!square_tower) {
            std::cerr << "Failed to cast IEntityRef to TowerRef" << std::endl;
            return 0;
        }
        square_tower->getPosition()->set<0>(towers_positions[i].x);
        square_tower->getPosition()->set<1>(towers_positions[i].y);
        square_tower->getSize()->set<0>(75);
        square_tower->getSize()->set<1>(75);
        square_tower->getSprite()->set<0>(assetsLoader.get_asset(EMPTY_TOWER).id);
        square_tower->getID()->set<0>(i);
        i++;
    }

    auto curr_time = std::chrono::steady_clock::now();
    PlayMusicStream(music);

    while (!WindowShouldClose()) {
        auto new_time = std::chrono::steady_clock::now();
        auto dt = std::chrono::duration<float>(new_time - curr_time).count();
        curr_time = new_time;

        UpdateMusicStream(music);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (mainMenu.settings.color_blind) {
            BeginShaderMode(mainMenu.colorblindnessShader);
        } else if (mainMenu.settings.color_blind_simulation) {
            BeginShaderMode(mainMenu.colorblindSimShader);
        }

        _eM.addTime(dt);
        EndShaderMode();

        if (towerClickSystem.open) {
            open_tower_menu(towerClickSystem);
        }

        EndDrawing();
    }
    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}