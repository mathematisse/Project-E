#include "Archetypes.hpp"
#include "MainMenu.hpp"
#include <cstdlib>
#include <chrono>
#include <string>

#include <iostream>
#include <raylib.h>
#include "NetworkManager.hpp"
#include "core/Core.hpp"
#include "lib_log/log.hpp"
#include "raygui.h"
#include "AssetsPath.hpp"

// ECS includes
#include "lib_ecs/Chunks/ChunkPos.hpp"
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

Vector2 get_player_position(ECS::EntityManager &_eM, ECS::Chunks::chunkPos_t &playerPos)
{
    auto player = _eM.getEntity<ECS::E::GameEntity>(playerPos);
    return {player.getPositionVal().get<0>(), player.getPositionVal().get<1>()};
}

// std::vector<Vector2> get_enemies_position(ECS::EntityManager &_eM)
// {
//     std::vector<Vector2> enemiesPos;
//     auto enemies = _eM.getEntities(ECS::C::ENT_ALIVE, ECS::C::ENT_ENEMY);
//     for (const auto &entity : enemies) {
//         auto ref = _eM.getEntity(entity);
//         auto square_enemy = static_cast<ECS::E::SquareRef *>(ref.get());
//         if (!square_enemy) {
//             std::cerr << "Failed to cast IArchetypeRef to SquareRef" << std::endl;
//             return {};
//         }
//         enemiesPos.push_back(
//             {static_cast<float>(*square_enemy->getPosition().get<0>()),
//              static_cast<float>(*square_enemy->getPosition().get<1>())}
//         );
//     }
//     return enemiesPos;
// }

bool player_is_alive(ECS::EntityManager &_eM, const ECS::Chunks::chunkPos_t &playerPos)
{
    return _eM.getEntity<ECS::E::GameEntity>(playerPos).getHealthVal() > 0;
}

void update_player_sprite(
    ECS::EntityManager &_eM, const ECS::Chunks::chunkPos_t &playerPos, AssetsLoader &assetsLoader
)
{
    auto player = _eM.getEntity<ECS::E::GameEntity>(playerPos);
    if (player.getHealthVal() == 1) {
        player.setSprite({assetsLoader.get_asset(P1VD).id});
    } else if (player.getHealthVal() == 2) {
        player.setSprite({assetsLoader.get_asset(P1D).id});
    } else if (player.getHealthVal() == 3) {
        player.setSprite({assetsLoader.get_asset(P1SD).id});
    } else if (player.getHealthVal() == 4) {
        player.setSprite({assetsLoader.get_asset(P1FR).id});
    }
}

void setup_decor(ECS::EntityManager &_eM, AssetsLoader &assetsLoader)
{
    size_t i = 0;
    for (auto background : _eM.createEntities<ECS::E::DecorEntity, 3>()) {
        background.setType({GameEntityType::BACKGROUND});
        background.setSize({3000, WINDOW_HEIGHT});
        background.setSprite({assetsLoader.get_asset(BACKGROUND_PATH).id});
        background.setPosition({(float) i * 3000.0F, 0});
        i++;
    }
    i = 0;
    for (auto ground : _eM.createEntities<ECS::E::DecorEntity, 250>()) {
        ground.setType({GameEntityType::WALL});
        ground.setSize({80, 100});
        ground.setPosition({(float) i * 80.0F, WINDOW_HEIGHT - 100});
        ground.setSprite({assetsLoader.get_asset(FLOOR).id});
        i++;
    }
    i = 0;
    for (auto ceiling : _eM.createEntities<ECS::E::DecorEntity, 250>()) {
        ceiling.setType({GameEntityType::WALL});
        ceiling.setSize({80, 100});
        ceiling.setPosition({(float) i * 80.0F, 0});
        ceiling.setSprite({assetsLoader.get_asset(CEILING).id});
        i++;
    }
}

ECS::Chunks::chunkPos_t setup_player(ECS::EntityManager &_eM, AssetsLoader &assetsLoader)
{
    auto engine = _eM.createEntity<ECS::E::AnimatedGameEntity>();
    engine.setType({GameEntityType::ENGINE});
    engine.setSize({80, 80});
    engine.setRotation({90});
    engine.setPosition({WINDOW_WIDTH / 4.0F, WINDOW_HEIGHT / 2.0F});
    engine.setAnimatedSprite({assetsLoader.get_asset(ENGINE_1).id, 4, 0, 8.0F});
    engine.setTimer();
    engine.setNetworkID();

    auto player = _eM.createEntity<ECS::E::GameEntity>();
    player.setPosition({WINDOW_WIDTH / 4.0F, WINDOW_HEIGHT / 2.0F});
    player.setType({GameEntityType::LPLAYER});
    player.setColor({255, 255, 255, 255});
    player.setCanShoot({0, 0.3F, 0.0F});
    player.setSize({80, 80});
    player.setRotation({90});
    player.setSprite({assetsLoader.get_asset(P1FR).id});
    player.setHealth({4});
    player.setNetworkID();

    return player.getChunkPosVal();
}

#define SPATIAL2D_SYS_GROUP "SPATIAL2D"

namespace engine::module {

class RTypeModule : public IModule {
protected:
public:
    RTypeModule() { }

    void load(ECS::EntityManager &entityManager) override { }
};

}

int main(int ac, char ** /*av*/)
{
    if (ac != 1) {
        std::cerr << "Usage: ./rtype_client" << std::endl;
        return 1;
    }
    LOG_SET_LEVEL(DEBUG);
    LOG_SET_STREAM(std::cerr);
    LOG_SET_FILE("rtype_client.log", true);
    srand(time(NULL));

    Camera2D camera = {};
    init_camera(camera);

    ECS::EntityManager _eM(FIXED_TIMESTEP);
    ECS::S::DestroyEntitiesSystem destroyEntitiesSystem(_eM);
    ECS::S::MoveOtherPlayerSystem moveOtherPlayerSystem;
    net::RTypeClient client(
        _eM, moveOtherPlayerSystem.playerStates, destroyEntitiesSystem.entitiesDestroyed,
        camera.target.x
    );

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "R-Type");
    InitAudioDevice();
    SetTargetFPS(60);

    AssetsLoader assetsLoader;
    assetsLoader.load_assets(paths);

    client.enemySpriteId = assetsLoader.get_asset(E1FC).id;
    client.frigateSpriteId = assetsLoader.get_asset(FRIGATE_1).id;
    client.bulletSpriteId = assetsLoader.get_asset(BASE_BULLET_PATH).id;
    client.bigShotSpriteId = assetsLoader.get_asset(BIG_SHOT_PATH).id;
    client.powerUpSpriteId = assetsLoader.get_asset(POWER_UP_PATH).id;

    MainMenu mainMenu(client, assetsLoader);

    if (!mainMenu.open()) {
        return 0;
    }
    Texture2D loading_background = assetsLoader.get_asset(LOADING_BACKGROUND);
    Music loading_music = LoadMusicStream(assetsLoader.get_real_path(LOADING_MUSIC).c_str());
    Music game_music = LoadMusicStream(assetsLoader.get_real_path(GAME_MUSIC).c_str());

    PlayMusicStream(loading_music);
    SetMusicVolume(loading_music, mainMenu.settings.volume / 100.0f);
    SetMusicVolume(game_music, mainMenu.settings.volume / 100.0f);
    bool change_music = false;

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
    ECS::E::AnimatedDecorEntity::Pool animatedDecorEntityPool(RTYPE_ENTITY_POOL_SIZE);
    _eM.registerEntityPool(&animatedDecorEntityPool);
    ECS::E::DecorEntity::Pool decorEntityPool(RTYPE_ENTITY_POOL_SIZE);
    _eM.registerEntityPool(&decorEntityPool);
    ECS::E::AnimatedGameEntity::Pool animatedGameEntityPool(RTYPE_ENTITY_POOL_SIZE);
    _eM.registerEntityPool(&animatedGameEntityPool);
    ECS::E::GameEntity::Pool gameEntityPool(RTYPE_ENTITY_POOL_SIZE);
    _eM.registerEntityPool(&gameEntityPool);

    setup_decor(_eM, assetsLoader);

    auto player = setup_player(_eM, assetsLoader);
    client.playerPos = player;

    Vector2 playerPosition = {0, 0};
    bool playerAlive = true;

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

        if (client.started) {
            update_camera(camera, dt);
        }
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
            auto engine = _eM.createEntity<ECS::E::AnimatedGameEntity>();
            engine.setType({GameEntityType::ENGINE});
            engine.setSize({80, 80});
            engine.setRotation({90});
            engine.setPosition({playerState.x, playerState.y});
            engine.setAnimatedSprite({assetsLoader.get_asset(ENGINE_1).id, 4, 0, 8.0F});
            engine.setTimer();
            engine.setNetworkID({playerState.netId});

            auto player = _eM.createEntity<ECS::E::GameEntity>();
            player.setPosition({playerState.x * 300.0F, playerState.y * 300.0F});
            player.setVelocity({playerState.vx * 300.0F, playerState.vy * 200.0F});
            player.setType({GameEntityType::PLAYER});
            player.setColor({255, 255, 255, 255});
            player.setWeapon({WeaponType::BIG_SHOT});
            player.setCanShoot(
                {true, player.getWeaponVal() == WeaponType::BIG_SHOT ? 1.5F : 0.3F, 0.0F}
            );
            player.setSize({80, 80});
            player.setRotation({90});
            player.setSprite({assetsLoader.get_asset(P2).id});
            player.setHealth({4});
            player.setNetworkID({playerState.netId});
        }
        moveOtherPlayerSystem.playerStates.clear();
        destroyEntitiesSystem.entitiesDestroyed.clear();
    }
    UnloadMusicStream(loading_music);
    UnloadMusicStream(game_music);
    CloseWindow();
    CloseAudioDevice();

    _eM.deleteEverything();
    return 0;
}
