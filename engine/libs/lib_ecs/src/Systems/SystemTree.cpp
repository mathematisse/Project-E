/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base SystemTree
*/

#include "Systems/SystemTree.hpp"
#include <iterator>
#include <utility>

#include <iostream>

#define BLUE "\033[34m"
#define RESET "\033[0m"

namespace ECS { namespace S {
SystemTreeNode::SystemTreeNode(int group,
  std::vector<ISystem *> startSystems,
  std::vector<ISystem *> endSystems,
  std::vector<SystemTreeNode> children)
  : _group(group), _startSystems(std::move(startSystems)), _children(std::move(children)),
    _endSystems(std::move(endSystems))
{}

bool SystemTreeNode::addSystemGroup(int group, int neighbourGroup, bool addBefore, bool addInside)
{
  if (group == _group && addInside) {
    if (addBefore) {
      _children.emplace(_children.begin(), neighbourGroup);
    } else {
      _children.emplace_back(neighbourGroup);
    }
    return true;
  }
  for (auto it = _children.begin(); it != _children.end(); ++it) {
    auto &child = *it;
    if (child.getGroup() == group && !addInside) {
      if (addBefore) {
        _children.insert(it, SystemTreeNode(neighbourGroup));
      } else {
        _children.insert(std::next(it), SystemTreeNode(neighbourGroup));
      }
      return true;
    }
  }
  return false;
}

bool SystemTreeNode::addSystem(ISystem *system, int group, bool atStart)
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
    if (child.addSystem(system, group, atStart)) { return true; }
  }
  return false;
}

void SystemTreeNode::registerEntityPool(E::IEntityPool *entityPool)
{
  for (auto &startSystem : _startSystems) { startSystem->tryAddEntityPool(entityPool); }
  for (auto &child : _children) { child.registerEntityPool(entityPool); }
  for (auto &endSystem : _endSystems) { endSystem->tryAddEntityPool(entityPool); }
}

void SystemTreeNode::runNode()
{
  std::cout << "\n" BLUE "[SYSTEM RUN] Running node " << _group << RESET "\n";
  for (auto &startSystem : _startSystems) { startSystem->run(); }
  for (auto &child : _children) { child.runNode(); }
  for (auto &endSystem : _endSystems) { endSystem->run(); }
}

int SystemTreeNode::getGroup() const { return _group; }

SystemTree::SystemTree()
  : _root(ROOTSYSGROUP, std::vector<ISystem *>(), std::vector<ISystem *>(), std::vector<SystemTreeNode>())
{}

SystemTree::~SystemTree() = default;

bool SystemTree::addSystemGroup(int group, int neighbourGroup, bool addBefore, bool addInside)
{
  return _root.addSystemGroup(group, neighbourGroup, addBefore, addInside);
}

bool SystemTree::addSystem(ISystem *system, int group, bool atStart) { return _root.addSystem(system, group, atStart); }

void SystemTree::registerEntityPool(E::IEntityPool *entityPool) { _root.registerEntityPool(entityPool); }

void SystemTree::runTree()
{
  std::cout << "\n" BLUE "----[START SYSTEM RUN]----" RESET "\n";
  _root.runNode();
  std::cout << "\n" BLUE "-----[END SYSTEM RUN]-----" RESET "\n";
}
} }// namespace ECS::S
