#include <iostream>
#include <chrono>
#include <raylib.h>
#include <raygui.h>
#include <vector>
#include <string>
#include "Components.hpp"
#include "core/Core.hpp"
#include "render/AssetsLoader.hpp"
#include "AssetsPath.hpp"
#include "MainMenu.hpp"

// ECS includes
#include "Archetypes.hpp"
#include "lib_ecs/EntityManager.hpp"
#include "lib_ecs/Systems/SystemTree.hpp"

#include "Systems.hpp"
#include "render/Render.hpp"
#include "spatial2d/Spatial2D.hpp"

void open_tower_menu(
    ECS::S::TowerClickSystem &towerClickSystem, size_t &money,
    std::array<tower_info, TOWER_COUNT> &towers_info
)
{
    Vector2 pos = {towerClickSystem.pos.x + 100, towerClickSystem.pos.y};
    static bool error_box = false;
    short cost = 0;

    if (towerClickSystem.selectedTower.type != TowerType::NONE) {
        GuiWindowBox(
            (Rectangle) {pos.x, pos.y, 200, 200},
            tower_names[(size_t) towerClickSystem.selectedTower.type].c_str()
        );
        DrawCircleLines(
            towerClickSystem.pos.x, towerClickSystem.pos.y,
            tower_range[(size_t) towerClickSystem.selectedTower.type]
                       [towerClickSystem.selectedTower.level - 1],
            WHITE
        );
        GuiLabel((Rectangle) {pos.x + 10, pos.y + 40, 100, 20}, "Level:");
        GuiLabel(
            (Rectangle) {pos.x + 100, pos.y + 40, 100, 20},
            std::to_string(towerClickSystem.selectedTower.level).c_str()
        );
        if (towerClickSystem.selectedTower.level < 3) {
            if (towerClickSystem.selectedTower.type == TowerType::ARCHER) {
                cost = 50;
            } else if (towerClickSystem.selectedTower.type == TowerType::WIZARD) {
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
                towerClickSystem.selectedTower.type = TowerType::ARCHER;
                towerClickSystem.selectedTower.level = 1;
                money -= 50;
            } else {
                error_box = true;
            }
        }
        GuiLabel((Rectangle) {pos.x + 20, pos.y + 105, 100, 20}, "80");
        if (GuiButton((Rectangle) {pos.x + 75, pos.y + 100, 150, 30}, "Wizard")) {
            if (money >= 80) {
                towerClickSystem.selectedTower.type = TowerType::WIZARD;
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

    // Engine modules
    engine::module::Core mCore;
    mCore.load(_eM);
    engine::module::Render mRender(assetsLoader);
    mRender.load(_eM);
    engine::module::Spatial2D mSpatial2D;
    mSpatial2D.load(_eM);

    ECS::E::DecorEntity::Pool decorPool(1024);
    ECS::E::TowerEntity::Pool towerPool(32);
    ECS::E::EnemyEntity::Pool enemyPool(1024);
    ECS::E::PlayerEntity::Pool playerPool(1);
    ECS::E::ProjectileEntity::Pool projectilePool(1024);

    ECS::S::TowerClickSystem towerClickSystem;
    ECS::S::ChangeTowerSprite changeTowerSprite(assetsLoader);
    ECS::S::SpawnEnemy spawnEnemy(assetsLoader, _eM);
    ECS::S::MoveEnemy moveEnemy;
    ECS::S::DamageEnemy damageEnemy(assetsLoader, _eM);
    ECS::S::KillProjectile killProjectile;
    ECS::S::DrawRotationProjectileSystem drawRotationProjectileSystem(assetsLoader);
    ECS::S::DestroyEntitiesSystem destroyEntitiesSystem(_eM);

    ECS::S::SystemTreeNode TDNode(
        "TDNode",
        {&killProjectile, &towerClickSystem, &changeTowerSprite, &spawnEnemy, &moveEnemy,
         &damageEnemy, &drawRotationProjectileSystem},
        {&destroyEntitiesSystem}
    );

    spawnEnemy.spriteId = assetsLoader.get_asset(GOBLIN).id;
    changeTowerSprite.spriteIds = {
        assetsLoader.get_asset(ARCHER1_TOWER).id, assetsLoader.get_asset(WIZARD1_TOWER).id,
        assetsLoader.get_asset(ARCHER2_TOWER).id, assetsLoader.get_asset(WIZARD2_TOWER).id,
        assetsLoader.get_asset(ARCHER3_TOWER).id, assetsLoader.get_asset(WIZARD3_TOWER).id
    };

    _eM.registerSystemNode(TDNode, ROOT_SYS_GROUP);

    _eM.registerEntityPool(&decorPool);
    _eM.registerEntityPool(&towerPool);
    _eM.registerEntityPool(&enemyPool);
    _eM.registerEntityPool(&playerPool);
    _eM.registerEntityPool(&projectilePool);

    auto bg = _eM.createEntity<ECS::E::DecorEntity>();
    bg.setSize({1920, 1080});
    bg.setPosition({1920.0F / 2, 1080.0F / 2});
    bg.setSprite(assetsLoader.get_asset(GAME_BACKGROUND).id);

    const std::vector<Vector2> towers_positions = {{1337, 274}, {1502, 939}, {666, 316},
                                                   {581, 609},  {951, 512},  {1189, 791},
                                                   {1062, 321}, {255, 330},  {732, 891}};
    int i = 0;
    std::array<tower_info, TOWER_COUNT> towers_info;

    for (auto tower : _eM.createEntities<ECS::E::TowerEntity, TOWER_COUNT>()) {
        tower.setPosition({towers_positions[i].x, towers_positions[i].y - 50});
        tower.setSize({120, 120});
        tower.setSprite(assetsLoader.get_asset(EMPTY_TOWER).id);
        tower.setID(i);
        tower_info towerInfo {};
        towerInfo.id = i;
        towerInfo.level = 0;
        towerInfo.type = TowerType::NONE;
        towerInfo.pos = {towers_positions[i].x, towers_positions[i].y - 50};
        towers_info[i] = towerInfo;
        i++;
    }

    _eM.createEntity<ECS::E::PlayerEntity>().setScore(0);

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

        spawnEnemy.delay += dt;
        spawnEnemy.kills = damageEnemy.kills;
        score += damageEnemy.kills;
        damageEnemy.kills = 0;
        damageEnemy.money = 0;

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

    _eM.deleteEverything();
    return 0;
}