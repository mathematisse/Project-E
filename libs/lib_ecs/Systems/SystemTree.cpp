/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base SystemTree
*/

#include "lib_ecs/Systems/SystemTree.hpp"
#include "lib_ecs/Systems/ExecutionTypes.hpp"
#include "lib_ecs/Systems/ThreadPool.hpp"
#include "lib_log/log.hpp"
#include <iterator>
#include <utility>

namespace ECS {
namespace S {
SystemTreeNode::SystemTreeNode(
    NodeExecutionType execType, std::string group, std::vector<ISystem *> startSystems,
    std::vector<ISystem *> endSystems, std::vector<SystemTreeNode> children
):
    _execType(execType),
    _group(std::move(group)),
    _startSystems(std::move(startSystems)),
    _children(std::move(children)),
    _endSystems(std::move(endSystems))
{
}

bool SystemTreeNode::addSystemGroup(
    NodeExecutionType execType, const std::string &targetGroup, const std::string &newGroup,
    bool addBefore, bool addInside
)
{
    // First check if are in a match case (will add in children)
    if (targetGroup == _group && addInside) {
        if (addBefore) {
            _children.emplace(_children.begin(), execType, newGroup);
        } else {
            _children.emplace_back(execType, newGroup);
        }
        return true;
    }
    // If not, check if we have to go deeper for insertion (could add in any sub-nodes)
    if (addInside) {
        for (auto &child : _children) {
            if (child.addSystemGroup(execType, targetGroup, newGroup, addBefore, addInside)) {
                return true;
            }
        }
        return false;
    }
    // Else check if we match with a child, to add as neighbour
    for (auto it = _children.begin(); it != _children.end(); ++it) {
        auto &child = *it;
        if (child.getGroup() != targetGroup) {
            continue;
        }
        if (addBefore) {
            _children.insert(it, SystemTreeNode(execType, newGroup));
        } else {
            _children.insert(std::next(it), SystemTreeNode(execType, newGroup));
        }
        return true;
    }
    return false;
}

bool SystemTreeNode::addSystem(ISystem *system, const std::string &group, bool atStart)
{
    if (group == _group) {
        if (atStart) {
            _startSystems.push_back(system);
        } else {
            _endSystems.push_back(system);
        }
        return true;
    }
    for (auto &child : _children) {
        if (child.addSystem(system, group, atStart)) {
            return true;
        }
    }
    return false;
}

bool SystemTreeNode::addSystemTreeNode(
    SystemTreeNode &node, const std::string &targetGroup, bool addBefore, bool addInside
)
{
    LOG_DEBUG("Adding system tree node with group " + node.getGroup());
    if (targetGroup == _group && addInside) {
        if (addBefore) {
            _children.insert(_children.begin(), node);
        } else {
            _children.push_back(node);
        }
        LOG_DEBUG("Successfully added in group " + _group);
        return true;
    }
    if (addInside) {
        for (auto &child : _children) {
            if (child.addSystemTreeNode(node, targetGroup, addBefore, addInside)) {
                LOG_DEBUG("Successfully added in group " + _group);
                return true;
            }
        }
        return false;
    }
    for (auto it = _children.begin(); it != _children.end(); ++it) {
        auto &child = *it;
        if (child.getGroup() != targetGroup) {
            continue;
        }
        if (addBefore) {
            _children.insert(it, node);
        } else {
            _children.insert(std::next(it), node);
        }
        LOG_DEBUG("Successfully added in group " + _group);
        return true;
    }
    return false;
}

void SystemTreeNode::registerEntityPool(E::IArchetypePool *entityPool)
{
    LOG_DEBUG("Registering entity pool in group " + _group);
    for (auto &startSystem : _startSystems) {
        startSystem->tryAddEntityPool(entityPool);
    }
    for (auto &child : _children) {
        child.registerEntityPool(entityPool);
    }
    for (auto &endSystem : _endSystems) {
        endSystem->tryAddEntityPool(entityPool);
    }
}

void SystemTreeNode::runNode(SystemTree &tree)
{
    if (startCallback) {
        startCallback(*this, tree);
    }
    if (!shouldRun) {
        return;
    }
    for (auto &startSystem : _startSystems) {
        startSystem->getRunStepData(tree);
        startSystem->run();
        if (_execType == SERIAL_NODE_EXECUTION) {
            globalThreadPool.waitAll();
        }
    }
    if (_execType == PARALLEL_NODE_EXECUTION) {
        globalThreadPool.waitAll();
    }
    for (auto &child : _children) {
        child.runNode(tree);
    }
    for (auto &endSystem : _endSystems) {
        endSystem->getRunStepData(tree);
        endSystem->run();
        if (_execType == SERIAL_NODE_EXECUTION) {
            globalThreadPool.waitAll();
        }
    }
    if (_execType == PARALLEL_NODE_EXECUTION) {
        globalThreadPool.waitAll();
    }
}

const std::string &SystemTreeNode::getGroup() const { return _group; }

std::vector<SystemTreeNode> &SystemTreeNode::getChildren() { return _children; }

SystemTreeNode::SystemTreeNode(const SystemTreeNode &node):
    _execType(node._execType),
    _group(node._group),
    _startSystems(node._startSystems),
    _children(node._children),
    _endSystems(node._endSystems)
{
}

SystemTreeNode &SystemTreeNode::operator=(const SystemTreeNode &node)
{
    _execType = node._execType;
    _group = node._group;
    _startSystems = node._startSystems;
    _children = node._children;
    _endSystems = node._endSystems;
    return *this;
}

SystemTree::SystemTree(const std::string &rootGroup):
    _root(
        SERIAL_NODE_EXECUTION, rootGroup, std::vector<ISystem *>(), std::vector<ISystem *>(),
        std::vector<SystemTreeNode>()
    )
{
}

SystemTree::~SystemTree() = default;

bool SystemTree::addSystemGroup(
    NodeExecutionType execType, const std::string &targetGroup, const std::string &newGroup,
    bool addBefore, bool addInside
)
{
    return _root.addSystemGroup(execType, targetGroup, newGroup, addBefore, addInside);
}

bool SystemTree::addSystem(ISystem *system, const std::string &group, bool atStart)
{
    return _root.addSystem(system, group, atStart);
}

bool SystemTree::addSystemTreeNode(
    SystemTreeNode &node, const std::string &targetGroup, bool addBefore, bool addInside
)
{
    return _root.addSystemTreeNode(node, targetGroup, addBefore, addInside);
}

void SystemTree::registerEntityPool(E::IArchetypePool *entityPool)
{
    _root.registerEntityPool(entityPool);
}

void SystemTree::runTree() { _root.runNode(*this); }
} // namespace S
} // namespace ECS
