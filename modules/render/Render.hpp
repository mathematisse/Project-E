#pragma once

#include "IModule.hpp"
#include "Components.hpp" // IWYU pragma: keep
#include "Systems.hpp"
#include "lib_ecs/Systems/SystemTree.hpp"
#include <cstddef>

#define ROOT_RENDER_SYS_GROUP "ROOT_RENDER"
#define RENDER_SYS_GROUP "RENDER"
#define DEBUG_RENDER_SYS_GROUP "DEBUG_RENDER"

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
        debugDrawSystems(),
        drawSpriteSystems(),
        drawAnimatedSpriteSystems(),
        spriteAnimationSystem(assetsLoader),

        rootRenderNode(RENDER_SYS_GROUP, {&spriteAnimationSystem})
    {
        renderNode = ECS::S::SystemTreeNode(RENDER_SYS_GROUP);
        debugRenderNode = ECS::S::SystemTreeNode(DEBUG_RENDER_SYS_GROUP);

        std::apply(
            [&camera](auto &...system) {
                int index = 0;
                ((system = ECS::S::DebugDrawSystem(index++, camera)), ...);
            },
            debugDrawSystems
        );

        std::apply(
            [&assetsLoader, &camera](auto &...system) {
                int index = 0;
                ((system = ECS::S::DrawSpriteSystem(index++, assetsLoader, camera)), ...);
            },
            drawSpriteSystems
        );

        std::apply(
            [&assetsLoader, &camera](auto &...system) {
                int index = 0;
                ((system = ECS::S::DrawAnimatedSpriteSystem(index++, assetsLoader, camera)), ...);
            },
            drawAnimatedSpriteSystems
        );

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
