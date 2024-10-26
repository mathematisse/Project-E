/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base SystemTree
*/

#pragma once

#include "lib_ecs/Systems/ISystem.hpp"
#include "lib_ecs/Systems/ASystemTree.hpp"

#define ROOT_SYS_GROUP "ROOT"
#define SYNC_SYS_GROUP "SYNC"

namespace ECS::S {
class SystemTree;

class SystemTreeNode {
public:
    explicit SystemTreeNode(
        const std::string group, std::vector<ISystem *> startSystems = std::vector<ISystem *>(),
        std::vector<ISystem *> endSystems = std::vector<ISystem *>(),
        std::vector<SystemTreeNode> children = std::vector<SystemTreeNode>()
    );
    ~SystemTreeNode() = default;
    bool addSystemGroup(
        const std::string &targetGroup, const std::string &newGroup, bool addBefore, bool addInside
    );
    bool addSystem(ISystem *system, const std::string &group, bool atStart);
    bool addSystemTreeNode(
        SystemTreeNode &node, const std::string &targetGroup, bool addBefore, bool addInside
    );
    void registerEntityPool(E::IEntityPool *entityPool);
    void runNode(SystemTree &tree);
    [[nodiscard]] const std::string &getGroup() const;

private:
    std::string _group;

    std::vector<ISystem *> _startSystems;
    std::vector<SystemTreeNode> _children;
    std::vector<ISystem *> _endSystems;
};

class SystemTree : public ASystemTree {
public:
    SystemTree();
    ~SystemTree();
    bool addSystemGroup(
        const std::string &targetGroup, const std::string &newGroup, bool addBefore, bool addInside
    );
    bool addSystem(ISystem *system, const std::string &group, bool atStart);
    bool addSystemTreeNode(
        SystemTreeNode &node, const std::string &targetGroup, bool addBefore, bool addInside
    );
    void registerEntityPool(E::IEntityPool *entityPool);
    void runTree();

private:
    SystemTreeNode _root;
};
} // namespace ECS::S
