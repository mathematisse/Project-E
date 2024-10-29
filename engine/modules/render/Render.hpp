#pragma once

#include "IModule.hpp"
#include "Components.hpp" // IWYU pragma: keep
#include "Systems.hpp"
#include "lib_ecs/Systems/SystemTree.hpp"

#define RENDER_SYS_GROUP "RENDER"
#define DEBUG_RENDER_SYS_GROUP "DEBUG_RENDER"

namespace engine::module {

class Render : public IModule {
    ECS::S::DebugDrawSystem debugDrawSystem;
    ECS::S::DrawSpriteSystem drawSpriteSystem;
    ECS::S::DrawAnimatedSpriteSystem drawAnimatedSpriteSystem;
    ECS::S::SpriteAnimationSystem spriteAnimationSystem;

    ECS::S::SystemTreeNode renderNode;
    ECS::S::SystemTreeNode debugRenderNode;

public:
    Render(Camera2D &camera, AssetsLoader &assetsLoader):
        debugDrawSystem(camera),
        drawSpriteSystem(assetsLoader, camera),
        drawAnimatedSpriteSystem(assetsLoader, camera),
        spriteAnimationSystem(assetsLoader),
        renderNode(
            RENDER_SYS_GROUP, {&drawSpriteSystem, &drawAnimatedSpriteSystem, &spriteAnimationSystem}
        ),
        debugRenderNode(DEBUG_RENDER_SYS_GROUP, {&debugDrawSystem})
    {
    }
    explicit Render(AssetsLoader &assetsLoader):
        debugDrawSystem(),
        drawSpriteSystem(assetsLoader),
        drawAnimatedSpriteSystem(assetsLoader),
        spriteAnimationSystem(assetsLoader),
        renderNode(
            RENDER_SYS_GROUP, {&drawSpriteSystem, &drawAnimatedSpriteSystem, &spriteAnimationSystem}
        ),
        debugRenderNode(DEBUG_RENDER_SYS_GROUP, {&debugDrawSystem})
    {
    }
    void load(ECS::EntityManager &entityManager) override
    {
        entityManager.registerSystemNode(renderNode, ROOT_SYS_GROUP);
    }
};

}
