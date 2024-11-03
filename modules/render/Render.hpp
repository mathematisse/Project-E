#pragma once

#include "IModule.hpp"
#include "Components.hpp" // IWYU pragma: keep
#include "Systems.hpp"
#include "lib_ecs/Systems/ExecutionTypes.hpp"
#include "lib_ecs/Systems/SystemTree.hpp"
#include "raylib.h"
#include <cstddef>

#define ROOT_RENDER_SYS_GROUP "ROOT_RENDER"
#define RENDER_SYS_GROUP "RENDER"
#define DEBUG_RENDER_SYS_GROUP "DEBUG_RENDER"

template<uint8_t Layers, typename Class, typename... Args, std::size_t... Is>
std::array<Class, Layers> makeSystemsHelper(std::index_sequence<Is...> /*unused*/, Args &...args)
{
    return {(static_cast<void>(Is), Class(Is, args...))...};
}

template<uint8_t Layers, typename Class, typename... Args>
std::array<Class, Layers> makeSystems(Args &...args)
{
    return makeSystemsHelper<Layers, Class, Args...>(std::make_index_sequence<Layers> {}, args...);
}

namespace engine::module {
template<uint8_t Layers>
class Render;
}

template<uint8_t Layers>
static void initRenderModule(engine::module::Render<Layers> &module)
{
    auto *rnode = module.rootRenderNode.getChildren()[0];
    auto *dnode = module.rootRenderNode.getChildren()[1];
    for (size_t i = 0; i < Layers; ++i) {
        rnode->addSystem(&module.drawSpriteSystems[i], RENDER_SYS_GROUP);
        rnode->addSystem(&module.drawAnimatedSpriteSystems[i], RENDER_SYS_GROUP);
    }
    std::apply(
        [&dnode](auto &...system) {
            ((dnode->addSystem(&system, DEBUG_RENDER_SYS_GROUP)), ...);
        },
        module.debugDrawSystems
    );
    dnode->shouldRun = false;
    dnode->startCallback = [&module](ECS::S::SystemTreeNode &node, ECS::S::SystemTree & /*tree*/) {
        if (IsKeyPressed(KEY_F3)) {
            module.debugRender = !module.debugRender;
        }
        node.shouldRun = module.debugRender;
    };
    rnode->startCallback = [&module](ECS::S::SystemTreeNode &node, ECS::S::SystemTree & /*tree*/) {
        node.shouldRun = !module.debugRender;
    };
}

namespace engine::module {

template<uint8_t Layers>
class Render : public IModule {
public:
    bool debugRender = false;

    Camera2D camera{};

    std::array<ECS::S::DebugDrawSystem, Layers> debugDrawSystems;
    std::array<ECS::S::DrawSpriteSystem, Layers> drawSpriteSystems;
    std::array<ECS::S::DrawAnimatedSpriteSystem, Layers> drawAnimatedSpriteSystems;
    ECS::S::SpriteAnimationSystem spriteAnimationSystem;

    ECS::S::SystemTreeNode renderNode;
    ECS::S::SystemTreeNode debugRenderNode;
    ECS::S::SystemTreeNode rootRenderNode;

    Render(AssetsLoader &assetsLoader, bool useCamera):
        debugDrawSystems(makeSystems<Layers, ECS::S::DebugDrawSystem>(camera, useCamera)),
        drawSpriteSystems(
            makeSystems<Layers, ECS::S::DrawSpriteSystem>(assetsLoader, camera, useCamera)
        ),
        drawAnimatedSpriteSystems(
            makeSystems<Layers, ECS::S::DrawAnimatedSpriteSystem>(assetsLoader, camera, useCamera)
        ),
        spriteAnimationSystem(assetsLoader),
        renderNode(ECS::S::SERIAL_NODE_EXECUTION, RENDER_SYS_GROUP),
        debugRenderNode(ECS::S::SERIAL_NODE_EXECUTION, DEBUG_RENDER_SYS_GROUP),
        rootRenderNode(
            ECS::S::SERIAL_NODE_EXECUTION, ROOT_RENDER_SYS_GROUP, {&spriteAnimationSystem}, {},
            {&renderNode, &debugRenderNode}
        )
    {
        initRenderModule(*this);
    }

    void load(ECS::EntityManager &entityManager) override
    {
        entityManager.registerSystemNode(&rootRenderNode, ROOT_SYS_GROUP);
    }
};

}
