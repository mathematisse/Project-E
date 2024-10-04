/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base SystemTree
*/

#pragma once

#include "Systems/ISystem.hpp"
#include <list>


namespace ECS::S {
enum PureSystemGroups {
  NONESYSGROUP = 0,
  ROOTSYSGROUP = 1,
  SYNCSYSGROUP = 2,
};

class SystemTreeNode
{
public:
  explicit SystemTreeNode(int group,
    std::vector<ISystem *> startSystems = std::vector<ISystem *>(),
    std::vector<ISystem *> endSystems = std::vector<ISystem *>(),
    std::vector<SystemTreeNode> children = std::vector<SystemTreeNode>());
  ~SystemTreeNode() = default;
  SystemTreeNode(const SystemTreeNode &other) = default;
  SystemTreeNode(SystemTreeNode &&other) = default;
  SystemTreeNode &operator=(const SystemTreeNode &other) = default;
  SystemTreeNode &operator=(SystemTreeNode &&other) = default;
  bool addSystemGroup(int group, int neighbourGroup, bool addBefore, bool addInside);
  bool addSystem(ISystem *system, int group, bool atStart);
  void registerEntityPool(E::IEntityPool *entityPool);
  void runNode();
  [[nodiscard]] int getGroup() const;

private:
  int _group;

  std::vector<ISystem *> _startSystems;
  std::vector<SystemTreeNode> _children;
  std::vector<ISystem *> _endSystems;
};


class SystemTree
{
public:
  SystemTree();
  ~SystemTree();
  SystemTree(const SystemTree &other) = default;
  SystemTree(SystemTree &&other) = default;
  SystemTree &operator=(const SystemTree &other) = default;
  SystemTree &operator=(SystemTree &&other) = default;
  bool addSystemGroup(int group, int neighbourGroup, bool addBefore, bool addInside);
  bool addSystem(ISystem *system, int group, bool atStart);
  bool addSystemTreeNodes(SystemTreeNode &node, int group, bool atStart);
  void registerEntityPool(E::IEntityPool *entityPool);
  void runTree();

private:
  SystemTreeNode _root;
};
}// namespace ECS::S
