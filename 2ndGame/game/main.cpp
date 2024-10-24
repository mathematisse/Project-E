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
#include "Enemy.hpp"
#include "Player.hpp"
#include "Tower.hpp"
#include "Projectile.hpp"
#include "Systems.hpp"

void open_tower_menu(
    ECS::S::TowerClickSystem &towerClickSystem, size_t &money, std::vector<tower_info> &towers_info
)
{
    Vector2 pos = {towerClickSystem.pos.x + 100, towerClickSystem.pos.y};
    static bool error_box = false;
    short cost = 0;

    if (towerClickSystem.selectedTower.type != NONE) {
        GuiWindowBox(
            (Rectangle) {pos.x, pos.y, 200, 200},
            tower_names[towerClickSystem.selectedTower.type].c_str()
        );
        DrawCircleLines(
            towerClickSystem.pos.x + 50, towerClickSystem.pos.y + 50,
            tower_range[towerClickSystem.selectedTower.type]
                       [towerClickSystem.selectedTower.level - 1],
            WHITE
        );
        GuiLabel((Rectangle) {pos.x + 10, pos.y + 40, 100, 20}, "Level:");
        GuiLabel(
            (Rectangle) {pos.x + 100, pos.y + 40, 100, 20},
            std::to_string(towerClickSystem.selectedTower.level).c_str()
        );
        if (towerClickSystem.selectedTower.level < 3) {
            if (towerClickSystem.selectedTower.type == ARCHER) {
                cost = 50;
            } else if (towerClickSystem.selectedTower.type == WIZARD) {
                cost = 80;
            }
            GuiLabel((Rectangle) {pos.x + 10, pos.y + 65, 100, 20}, "Price:");
            GuiLabel(
                (Rectangle) {pos.x + 100, pos.y + 65, 100, 20},
                std::to_string((towerClickSystem.selectedTower.level * cost)).c_str()
            );
            if (GuiButton((Rectangle) {pos.x + 25, pos.y + 100, 150, 30}, "Upgrade")) {
                if (money >= towerClickSystem.selectedTower.level * cost) {
                    money -= towerClickSystem.selectedTower.level * cost;
                    towerClickSystem.selectedTower.level++;
                }
            }
        } else {
            GuiLabel((Rectangle) {pos.x + 25, pos.y + 100, 150, 30}, "Max level");
        }
    } else {
        GuiWindowBox((Rectangle) {pos.x, pos.y, 300, 200}, "Tower");
        GuiLabel((Rectangle) {pos.x + 20, pos.y + 55, 100, 20}, "50");
        if (GuiButton((Rectangle) {pos.x + 75, pos.y + 50, 150, 30}, "Archer")) {
            if (money >= 50) {
                towerClickSystem.selectedTower.type = ARCHER;
                towerClickSystem.selectedTower.level = 1;
                money -= 50;
            } else {
                error_box = true;
            }
        }
        GuiLabel((Rectangle) {pos.x + 20, pos.y + 105, 100, 20}, "80");
        if (GuiButton((Rectangle) {pos.x + 75, pos.y + 100, 150, 30}, "Wizard")) {
            if (money >= 80) {
                towerClickSystem.selectedTower.type = WIZARD;
                towerClickSystem.selectedTower.level = 1;
                money -= 80;
            } else {
                error_box = true;
            }
        }
    }
    if (GuiButton((Rectangle) {pos.x + 50, pos.y + 150, 100, 30}, "Quit")) {
        towerClickSystem.open = false;
        error_box = false;
    }
    if (error_box) {
        GuiWindowBox((Rectangle) {pos.x, pos.y - 150, 300, 100}, "Not enough money");
        if (GuiButton((Rectangle) {pos.x + 50, pos.y - 120, 150, 50}, "OK")) {
            error_box = false;
        }
    }
    for (auto &tower : towers_info) {
        if (tower.id == towerClickSystem.selectedTower.id) {
            tower.level = towerClickSystem.selectedTower.level;
            tower.type = towerClickSystem.selectedTower.type;
        }
    }
}

int main(int ac, char *av[])
{
    if (ac != 1) {
        std::cerr << "Usage: ./tower_defense" << std::endl;
        return 1;
    }

    srand(time(NULL));
    AssetsLoader assetsLoader;
    ECS::EntityManager _eM;

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Tower Defense");
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
    ECS::E::EnemyPool enemyPool;
    ECS::E::PlayerPool playerPool;
    ECS::E::ProjectilePool projectilePool;
    ECS::S::DrawSpriteSystem drawSpriteSystem(assetsLoader);
    ECS::S::TowerClickSystem towerClickSystem;
    ECS::S::ChangeTowerSprite changeTowerSprite(assetsLoader);
    ECS::S::ApplyVelocitySystem applyVelocitySystem;
    ECS::S::SpawnEnemy spawnEnemy(assetsLoader, _eM);
    ECS::S::MoveEnemy moveEnemy;
    ECS::S::DamageEnemy damageEnemy(assetsLoader, _eM);
    ECS::S::KillProjectile killProjectile;

    ECS::S::SystemTreeNode demoNode(
        42,
        {&killProjectile, &towerClickSystem, &changeTowerSprite, &spawnEnemy, &moveEnemy,
         &applyVelocitySystem, &damageEnemy, &drawSpriteSystem},
        {}
    );

    _eM.registerSystemNode(demoNode, ECS::S::ROOTSYSGROUP, false, true);

    _eM.registerEntityPool(&decorPool);
    _eM.registerEntityPool(&towerPool);
    _eM.registerEntityPool(&enemyPool);
    _eM.registerEntityPool(&playerPool);
    _eM.registerEntityPool(&projectilePool);

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

    const std::vector<Vector2> towers_positions = {{1287, 224}, {1452, 889}, {616, 266},
                                                   {531, 559},  {901, 462},  {1139, 741},
                                                   {1012, 271}, {205, 280},  {682, 841}};

    int i = 0;

    std::vector<tower_info> towers_info;

    for (const auto &entity : towers) {
        auto ref = _eM.getEntity(entity);

        auto square_tower = dynamic_cast<ECS::E::TowerRef *>(ref.get());
        if (!square_tower) {
            std::cerr << "Failed to cast IEntityRef to TowerRef" << std::endl;
            return 0;
        }
        square_tower->getPosition()->set<0>(towers_positions[i].x);
        square_tower->getPosition()->set<1>(towers_positions[i].y - 50);
        square_tower->getSize()->set<0>(75);
        square_tower->getSize()->set<1>(75);
        square_tower->getSprite()->set<0>(assetsLoader.get_asset(EMPTY_TOWER).id);
        square_tower->getID()->set<0>(i);
        tower_info tower;
        tower.id = i;
        tower.level = 0;
        tower.type = NONE;
        tower.pos = {towers_positions[i].x, towers_positions[i].y - 50};
        towers_info.push_back(tower);
        i++;
    }

    auto player = _eM.createEntity("Player", ECS::C::ENT_ALIVE);

    auto playerRef = _eM.getEntity(player);

    auto square_player = dynamic_cast<ECS::E::PlayerRef *>(playerRef.get());
    if (!square_player) {
        std::cerr << "Failed to cast IEntityRef to PlayerRef" << std::endl;
        return 0;
    }

    square_player->getScore()->set<0>(0);

    size_t money = 100;
    int player_health = 10;
    size_t score = 0;

    moveEnemy.player_health = player_health;
    damageEnemy.towers = towers_info;

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

        if (player_health <= 0) {
            ClearBackground(BLACK);
            DrawText("Game Over", 1920 / 2 - 250, 1080 / 2 - 200, 100, WHITE);
            DrawText("Score:", 1920 / 2 - 100, 1080 / 2, 50, WHITE);
            DrawText(std::to_string(score).c_str(), 1920 / 2 + 100, 1080 / 2, 50, WHITE);
            EndShaderMode();
            EndDrawing();
            continue;
        }

        applyVelocitySystem.deltaTime = dt;
        spawnEnemy.delay += dt;
        spawnEnemy.kills = damageEnemy.kills;
        score += damageEnemy.kills;
        damageEnemy.kills = 0;
        damageEnemy.money = 0;
        killProjectile.deltaTime = dt;

        _eM.addTime(dt);

        money += damageEnemy.money;

        player_health = moveEnemy.player_health;

        DrawRectangle(5, 5, 200, 100, {255, 255, 255, 200});
        DrawText("Money: ", 10, 10, 35, YELLOW);
        DrawText(std::to_string(money).c_str(), 130, 13, 35, YELLOW);
        DrawText("Health: ", 10, 50, 35, RED);
        DrawText(std::to_string(player_health).c_str(), 130, 53, 35, RED);

        DrawRectangle(1700, 5, 200, 50, {255, 255, 255, 200});
        DrawText("Score: ", 1705, 10, 35, BLACK);
        DrawText(std::to_string(score).c_str(), 1830, 13, 35, BLACK);

        EndShaderMode();

        if (towerClickSystem.open) {
            open_tower_menu(towerClickSystem, money, damageEnemy.towers);
        }

        EndDrawing();
        for (auto &tower : damageEnemy.towers) {
            tower.delay += dt;
        }
    }
    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}