#pragma once

#include "IModule.hpp"
#include "Components.hpp" // IWYU pragma: keep
#include "Systems.hpp"
#include "lib_ecs/Systems/SystemTree.hpp"
#include <cstddef>

#define ROOT_RENDER_SYS_GROUP "ROOT_RENDER"
#define RENDER_SYS_GROUP "RENDER"
#define DEBUG_RENDER_SYS_GROUP "DEBUG_RENDER"


template <uint8_t Layers, typename Class, std::size_t... Is, typename... Args>
std::array<Class, Layers> makeSystemsHelper(std::index_sequence<Is...> /*unused*/, Args... args)
{
    return { (static_cast<void>(Is), Class(args...))... };
}

template <uint8_t Layers, typename Class, typename... Args>
std::array<Class, Layers> makeSystems(Args... args)
{
    return makeSystemsHelper(std::make_index_sequence<Layers>{}, args...);
}

namespace engine::module {

template<uint8_t Layers>
class Render : public IModule {
    std::array<ECS::S::DebugDrawSystem, Layers> debugDrawSystems;
    std::array<ECS::S::DrawSpriteSystem, Layers> drawSpriteSystems;
    std::array<ECS::S::DrawAnimatedSpriteSystem, Layers> drawAnimatedSpriteSystems;
    ECS::S::SpriteAnimationSystem spriteAnimationSystem;

    ECS::S::SystemTreeNode rootRenderNode;
    ECS::S::SystemTreeNode renderNode;
    ECS::S::SystemTreeNode debugRenderNode;

    bool _debug = false;

public:
    Render(Camera2D &camera, AssetsLoader &assetsLoader):
        debugDrawSystems(makeSystems<Layers, ECS::S::DebugDrawSystem>(camera)),
        drawSpriteSystems(makeSystems<Layers, ECS::S::DrawSpriteSystem>(assetsLoader, camera)),
        drawAnimatedSpriteSystems(makeSystems<Layers, ECS::S::DrawAnimatedSpriteSystem>(assetsLoader, camera)),
        spriteAnimationSystem(assetsLoader),
        rootRenderNode(RENDER_SYS_GROUP, {&spriteAnimationSystem})
    {
        renderNode = ECS::S::SystemTreeNode(RENDER_SYS_GROUP);
        debugRenderNode = ECS::S::SystemTreeNode(DEBUG_RENDER_SYS_GROUP);

        for (size_t i = 0; i < Layers; ++i) {
            renderNode.addSystem(&drawSpriteSystems[i], RENDER_SYS_GROUP);
            renderNode.addSystem(&drawAnimatedSpriteSystems[i], RENDER_SYS_GROUP);
        }

        std::apply(
            [this](auto &...system) {
                ((debugRenderNode.addSystem(&system, DEBUG_RENDER_SYS_GROUP)), ...);
            },
            debugDrawSystems
        );

        debugRenderNode.shouldRun = false;

        debugRenderNode.startCallback =
            [this](ECS::S::SystemTreeNode &node, ECS::S::SystemTree & /*tree*/) {
                if (IsKeyPressed(KEY_F3)) {
                    this->_debug = !this->_debug;
                }

                node.shouldRun = this->_debug;
            };
        renderNode.startCallback =
            [this](ECS::S::SystemTreeNode &node, ECS::S::SystemTree & /*tree*/) {
                node.shouldRun = !this->_debug;
            };

        rootRenderNode.addSystemTreeNode(renderNode, ROOT_RENDER_SYS_GROUP, false, true);
        rootRenderNode.addSystemTreeNode(debugRenderNode, ROOT_RENDER_SYS_GROUP, false, true);
    }
    explicit Render(AssetsLoader &assetsLoader):
        Render(ECS::S::dummyCamera2D, assetsLoader)
    {
    }

    void load(ECS::EntityManager &entityManager) override
    {
        entityManager.registerSystemNode(renderNode, ROOT_SYS_GROUP);
        entityManager.registerSystemNode(debugRenderNode, ROOT_SYS_GROUP);
    }
};

}
