#include "DecorEntities.hpp"
#include "MainMenu.hpp"
#include <cstdlib>
#include <chrono>
#include <string>

#include <iostream>
#include <raylib.h>
#include "core/Core.hpp"
#include "lib_log/log.hpp"
#include "raygui.h"
#include <thread>
#include "AssetsPath.hpp"
#include "MainMenu.hpp"

// ECS includes
#include "lib_ecs/Chunks/ChunkPos.hpp"
#include "lib_ecs/Components/PureComponentPools.hpp"
#include "GameEntities.hpp"
#include "Systems.hpp"
#include "ClientSystems.hpp"
#include "DrawSystems.hpp"
#include "lib_ecs/EntityManager.hpp"
#include "lib_ecs/Systems/SystemTree.hpp"
#include "RTypeClient.hpp"
#include "render/Render.hpp"
#include "spatial2d/Spatial2D.hpp"

void init_camera(Camera2D &camera)
{
    camera.target = {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};
    camera.offset = {WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

void update_camera(Camera2D &camera, float dt)
{
    Vector2 old = camera.target;
    camera.target = {old.x + 80 * dt, WINDOW_HEIGHT / 2};
}

Vector2 get_player_position(ECS::EntityManager &_eM, ECS::Chunks::cPosArr_t &chunks)
{
    auto player = chunks;
    if (player.empty()) {
        return {0, 0};
    }
    auto ref = _eM.getEntity(player[0]);
    auto square_player = dynamic_cast<ECS::E::GameEntityRef *>(ref.get());
    if (!square_player) {
        LOG_ERROR("Failed to cast IEntityRef to GameEntityRef");
        return {0, 0};
    }
    return {*square_player->getPosition().get<0>(), *square_player->getPosition().get<1>()};
}

// std::vector<Vector2> get_enemies_position(ECS::EntityManager &_eM)
// {
//     std::vector<Vector2> enemiesPos;
//     auto enemies = _eM.getEntities(ECS::C::ENT_ALIVE, ECS::C::ENT_ENEMY);
//     for (const auto &entity : enemies) {
//         auto ref = _eM.getEntity(entity);
//         auto square_enemy = dynamic_cast<ECS::E::SquareRef *>(ref.get());
//         if (!square_enemy) {
//             std::cerr << "Failed to cast IEntityRef to SquareRef" << std::endl;
//             return {};
//         }
//         enemiesPos.push_back(
//             {static_cast<float>(*square_enemy->getPosition().get<0>()),
//              static_cast<float>(*square_enemy->getPosition().get<1>())}
//         );
//     }
//     return enemiesPos;
// }

char player_is_alive(ECS::EntityManager &_eM, ECS::Chunks::cPosArr_t &chunks)
{
    auto player = chunks;
    if (player.empty()) {
        LOG_INFO("Player is dead");
        return 0;
    }
    auto ref = _eM.getEntity(player[0]);
    auto square_player = dynamic_cast<ECS::E::GameEntityRef *>(ref.get());
    if (!square_player) {
        LOG_ERROR("Failed to cast IEntityRef to GameEntityRef");
        return 0;
    }
    return *square_player->getHealth().get<0>();
}

void update_player_sprite(
    ECS::EntityManager &_eM, ECS::Chunks::cPosArr_t &chunks, AssetsLoader &assetsLoader
)
{
    auto player = chunks;
    if (player.empty()) {
        return;
    }
    auto ref = _eM.getEntity(player[0]);
    auto square_player = dynamic_cast<ECS::E::GameEntityRef *>(ref.get());
    if (!square_player) {
        LOG_ERROR("Failed to cast IEntityRef to GameEntityRef");
        return;
    }
    if (*square_player->getHealth().get<0>() == 1) {
        square_player->setSprite({assetsLoader.get_asset(P1VD).id});
    } else if (*square_player->getHealth().get<0>() == 2) {
        square_player->setSprite({assetsLoader.get_asset(P1D).id});
    } else if (*square_player->getHealth().get<0>() == 3) {
        square_player->setSprite({assetsLoader.get_asset(P1SD).id});
    } else if (*square_player->getHealth().get<0>() == 4) {
        square_player->setSprite({assetsLoader.get_asset(P1FR).id});
    }
}

void setup_decor(
    ECS::EntityManager &_eM, NetworkManager &networkManager, AssetsLoader &assetsLoader
)
{

    auto background = _eM.createEntities("DecorEntity", 1);

    for (const auto &entity : background) {
        auto ref = _eM.getEntity(entity);

        auto square_background = dynamic_cast<ECS::E::DecorEntityRef *>(ref.get());
        if (!square_background) {
            LOG_ERROR("Failed to cast IEntityRef to DecorEntityRef");
            return;
        }
        square_background->setType({GameEntityType::BACKGROUND});
        square_background->setSize({3000, WINDOW_HEIGHT});
        square_background->setSprite({assetsLoader.get_asset(BACKGROUND_PATH).id});
    }

    auto ground = _eM.createEntities("DecorEntity", 250);

    int i = 0;
    for (const auto &entity : ground) {
        auto ref = _eM.getEntity(entity);

        auto *square_ground = dynamic_cast<ECS::E::DecorEntityRef *>(ref.get());
        if (!square_ground) {
            LOG_ERROR("Failed to cast IEntityRef to DecorEntityRef");
            return;
        }
        square_ground->setType({GameEntityType::WALL});
        square_ground->setSize({80, 100});
        square_ground->setPosition({(float) i * 80.0F, WINDOW_HEIGHT - 100});
        square_ground->setSprite({assetsLoader.get_asset(FLOOR).id});
        i++;
    }

    auto ceiling = _eM.createEntities("DecorEntity", 250);

    i = 0;
    for (const auto &entity : ceiling) {
        auto ref = _eM.getEntity(entity);

        auto square_ceiling = dynamic_cast<ECS::E::DecorEntityRef *>(ref.get());
        if (!square_ceiling) {
            LOG_ERROR("Failed to cast IEntityRef to DecorEntityRef");
            return;
        }
        square_ceiling->setType({GameEntityType::WALL});
        square_ceiling->setSize({80, 100});
        square_ceiling->setPosition({(float) i * 80.0F, 0});
        square_ceiling->setSprite({assetsLoader.get_asset(CEILING).id});
        i++;
    }
}

ECS::Chunks::cPosArr_t setup_player(ECS::EntityManager &_eM, AssetsLoader &assetsLoader)
{
    auto engine = _eM.createEntities("GameAnimatedEntity", 1);

    for (const auto &entity : engine) {
        auto ref = _eM.getEntity(entity);

        auto *square_engine = dynamic_cast<ECS::E::GameAnimatedEntityRef *>(ref.get());
        if (square_engine == nullptr) {
            LOG_ERROR("Failed to cast IEntityRef to GameEntityRef");
            return {};
        }
        square_engine->setType({GameEntityType::ENGINE});
        square_engine->setSize({80, 80});
        square_engine->setRotation({90});
        square_engine->setPosition({WINDOW_WIDTH / 4.0F, WINDOW_HEIGHT / 2.0F});
        square_engine->setAnimatedSprite({assetsLoader.get_asset(ENGINE_1).id, 4, 0, 8.0F});
        square_engine->setTimer({0.0F});
        square_engine->setNetworkID({0});
    }

    auto player = _eM.createEntities("GameEntity", 1);

    for (const auto &entity : player) {
        auto ref = _eM.getEntity(entity);

        auto *square_player = dynamic_cast<ECS::E::GameEntityRef *>(ref.get());
        if (square_player == nullptr) {
            LOG_ERROR("Failed to cast IEntityRef to GameEntityRef");
            return {};
        }
        square_player->setPosition({WINDOW_WIDTH / 4.0F, WINDOW_HEIGHT / 2.0F});
        square_player->setType({GameEntityType::LPLAYER});
        square_player->setColor({255, 255, 255, 255});
        square_player->setCanShoot({0, 0.3F, 0.0F});
        square_player->setSize({80, 80});
        square_player->setRotation({90});
        square_player->setSprite({assetsLoader.get_asset(P1FR).id});
        square_player->setHealth({4});
        square_player->setNetworkID({0});
    }
    return player;
}

int main(int ac, char **av)
{
    LOG_SET_LEVEL(DEBUG);
    LOG_SET_STREAM(std::cerr);
    LOG_SET_FILE("rtype_client.log", true);

    if (ac != 1) {
        std::cerr << "Usage: ./rtype_client" << std::endl;
        return 1;
    }

    Camera2D camera = {};
    init_camera(camera);

    srand(time(NULL));
    AssetsLoader assetsLoader;
    ECS::EntityManager _eM(FIXED_TIMESTEP);
    ECS::S::MoveOtherPlayerSystem moveOtherPlayerSystem;
    ECS::S::DestroyEntitiesSystem destroyEntitiesSystem(_eM);
    net::RTypeClient client(
        _eM, moveOtherPlayerSystem.playerStates, destroyEntitiesSystem.entitiesDestroyed,
        camera.target.x
    );

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "R-Type");
    InitAudioDevice();
    SetTargetFPS(60);

    assetsLoader.load_assets(paths);

    client.ennemySpriteId = assetsLoader.get_asset(E1FC).id;
    client.frigateSpriteId = assetsLoader.get_asset(FRIGATE_1).id;
    client.bulletSpriteId = assetsLoader.get_asset(BASE_BULLET_PATH).id;
    client.bigShotSpriteId = assetsLoader.get_asset(BIG_SHOT_PATH).id;

    MainMenu mainMenu(client, assetsLoader);

    if (!mainMenu.open())
        return 0;
    Texture2D loading_background = assetsLoader.get_asset(LOADING_BACKGROUND);
    Music loading_music = LoadMusicStream(assetsLoader.get_real_path(LOADING_MUSIC).c_str());
    Music game_music = LoadMusicStream(assetsLoader.get_real_path(GAME_MUSIC).c_str());

    PlayMusicStream(loading_music);
    SetMusicVolume(loading_music, mainMenu.settings.volume / 100.0f);
    SetMusicVolume(game_music, mainMenu.settings.volume / 100.0f);
    bool change_music = false;

    NetworkManager networkManager;

    // Engine modules
    engine::module::Core mCore;
    mCore.load(_eM);
    engine::module::Render mRender(camera, assetsLoader);
    mRender.load(_eM);
    engine::module::Spatial2D mSpatial2D;
    mSpatial2D.load(_eM);

    // Rtype systems
    ECS::S::ShowInfoSystem showInfoSystem(camera);
    ECS::S::UpdateEnginePosition updateEnginePosition;
    ECS::S::MovePlayerSystem moveSystem(client);
    moveSystem.auto_shoot = mainMenu.settings.auto_shoot;
    ECS::S::MoveBackgroundSystem moveBackgroundSystem;
    ECS::S::MoveEnnemySystem moveEnnemySystem;
    ECS::S::GetPlayerPositionSystem getPlayerPositionSystem;

    // System nodes
    ECS::S::SystemTreeNode rTypeFixedNode(
        "RtypeFixedNode",
        {&moveBackgroundSystem, &moveEnnemySystem, &updateEnginePosition, &moveSystem}
    );
    bool success = _eM.registerFixedSystemNode(rTypeFixedNode, ROOT_SYS_GROUP);
    ECS::S::SystemTreeNode rTypeNode(
        "RtypeNode",
        {&moveOtherPlayerSystem, &destroyEntitiesSystem, &getPlayerPositionSystem, &showInfoSystem}
    );
    if (success) {
        LOG_INFO("Successfully registered RtypeFixedNode");
    } else {
        LOG_ERROR("Failed to register RtypeFixedNode");
    }
    success = _eM.registerSystemNode(rTypeNode, ROOT_SYS_GROUP);
    if (success) {
        LOG_INFO("Successfully registered RtypeNode");
    } else {
        LOG_ERROR("Failed to register RtypeNode");
    }

    // Entity pools
    ECS::E::GameAnimatedEntityPool gameAnimatedEntityPool;
    _eM.registerEntityPool(&gameAnimatedEntityPool);
    ECS::E::GameEntityPool gameEntityPool;
    _eM.registerEntityPool(&gameEntityPool);
    ECS::E::DecorAnimatedEntityPool decorAnimatedEntityPool;
    _eM.registerEntityPool(&decorAnimatedEntityPool);
    ECS::E::DecorEntityPool decorEntityPool;
    _eM.registerEntityPool(&decorEntityPool);

    setup_decor(_eM, networkManager, assetsLoader);

    auto player = setup_player(_eM, assetsLoader);
    client.playerPos = player[0];

    Vector2 playerPosition = {0, 0};
    char playerAlive = 1;

    auto curr_time = std::chrono::steady_clock::now();

    size_t frame = 0;

    while (!WindowShouldClose()) {
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        client.update();
        moveSystem.frame = frame;
        auto new_time = std::chrono::steady_clock::now();
        auto dt = std::chrono::duration<float>(new_time - curr_time).count();
        curr_time = new_time;

        if (mainMenu.settings.color_blind) {
            BeginShaderMode(mainMenu.colorblindnessShader);
        }

        if (mainMenu.settings.color_blind_simulation) {
            BeginShaderMode(mainMenu.colorblindSimShader);
        }

        if (client.started)
            update_camera(camera, dt);
        moveBackgroundSystem.cameraX = camera.target.x;
        update_player_sprite(_eM, player, assetsLoader);

        playerPosition = get_player_position(_eM, player);
        moveEnnemySystem.playersPos = getPlayerPositionSystem.playersPos;
        getPlayerPositionSystem.playersPos.clear();
        updateEnginePosition.playerPosition = playerPosition;

        playerAlive = player_is_alive(_eM, player);
        updateEnginePosition.playerAlive = playerAlive;

        showInfoSystem.one_time = false;
        if (client.started && !change_music) {
            StopMusicStream(loading_music);
            PlayMusicStream(game_music);
            change_music = true;
        }
        if (!change_music) {
            UpdateMusicStream(loading_music);
        } else {
            UpdateMusicStream(game_music);
        }
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            if (client.started) {
                if (_eM.addTime(dt)) {
                    frame++;
                }
                EndMode2D();
            } else {
                DrawTexture(loading_background, 0, 0, WHITE);
                DrawText(
                    "Waiting for other players...", WINDOW_WIDTH / 2 - 500, WINDOW_HEIGHT / 2, 80,
                    WHITE
                );
            }
        }
        if (mainMenu.settings.color_blind) {
            EndShaderMode();
        }
        EndDrawing();

        // create palyer for each player states left over
        for (auto &playerState : moveOtherPlayerSystem.playerStates) {
            LOG_DEBUG(
                "Creating player for player state and netid: " + std::to_string(playerState.netId)
            );
            auto engine = _eM.createEntities("GameAnimatedEntity", 1);
            for (const auto &entity : engine) {
                auto ref = _eM.getEntity(entity);

                auto *square_engine = dynamic_cast<ECS::E::GameAnimatedEntityRef *>(ref.get());
                if (square_engine == nullptr) {
                    LOG_ERROR("Failed to cast IEntityRef to GameEntityRef");
                    return 1;
                }
                square_engine->setType({GameEntityType::ENGINE});
                square_engine->setSize({80, 80});
                square_engine->setRotation({90});
                square_engine->setPosition({playerState.x, playerState.y});
                square_engine->setAnimatedSprite({assetsLoader.get_asset(ENGINE_1).id, 4, 0, 8.0F});
                square_engine->setTimer({0.0F});
                square_engine->setNetworkID({playerState.netId});
            }
            auto player = _eM.createEntities("GameEntity", 1);
            for (const auto &entity : player) {
                auto ref = _eM.getEntity(entity);

                auto *square_player = dynamic_cast<ECS::E::GameEntityRef *>(ref.get());
                if (square_player == nullptr) {
                    LOG_ERROR("Failed to cast IEntityRef to GameEntityRef");
                    return 1;
                }
                square_player->setPosition({playerState.x * 300.0F, playerState.y * 300.0F});
                square_player->setVelocity({playerState.vx * 300.0F, playerState.vy * 200.0F});
                square_player->setType({GameEntityType::PLAYER});
                square_player->setColor({255, 255, 255, 255});
                square_player->setWeapon({WeaponType::BIG_SHOT});
                square_player->setCanShoot(
                    {true,
                     *square_player->getWeapon().get<0>() == WeaponType::BIG_SHOT ? 1.5F : 0.3F,
                     0.0F}
                );
                square_player->setSize({80, 80});
                square_player->setRotation({90});
                square_player->setSprite({assetsLoader.get_asset(P2).id});
                square_player->setHealth({4});
                square_player->setNetworkID({playerState.netId});
            }
        }
        moveOtherPlayerSystem.playerStates.clear();
        destroyEntitiesSystem.entitiesDestroyed.clear();
    }
    UnloadMusicStream(loading_music);
    UnloadMusicStream(game_music);
    CloseWindow();
    CloseAudioDevice();
    return 0;
}
