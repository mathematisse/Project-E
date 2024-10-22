#include <iostream>
#include <chrono>
#include <raylib.h>
#include "AssetsLoader.hpp"
#include "AssetsPath.hpp"
#include "MainMenu.hpp"

// ECS includes
#include "lib_ecs/Chunks/ChunkPos.hpp"
#include "lib_ecs/Components/PureComponentPools.hpp"
#include "lib_ecs/EntityManager.hpp"
#include "lib_ecs/Systems/SystemTree.hpp"

#include "Decor.hpp"
#include "DrawSystems.hpp"

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

    MainMenu mainMenu(assetsLoader);

    if (!mainMenu.open()) {
        return 0;
    }

    ECS::E::DecorPool decorPool;
    ECS::S::DrawSpriteSystem drawSpriteSystem(assetsLoader);

    ECS::S::SystemTreeNode demoNode(42, {&drawSpriteSystem}, {});

    _eM.registerSystemNode(demoNode, ECS::S::ROOTSYSGROUP, false, true);

    _eM.registerEntityPool(&decorPool);

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

    auto curr_time = std::chrono::steady_clock::now();

    while (!WindowShouldClose()) {
        auto new_time = std::chrono::steady_clock::now();
        auto dt = std::chrono::duration<float>(new_time - curr_time).count();
        curr_time = new_time;

        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (mainMenu.settings.color_blind) {
            BeginShaderMode(mainMenu.colorblindnessShader);
        }
        if (mainMenu.settings.color_blind_simulation) {
            BeginShaderMode(mainMenu.colorblindSimShader);
        }

        _eM.addTime(dt);

        EndShaderMode();
        EndShaderMode();

        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();
}