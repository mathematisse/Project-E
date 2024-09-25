/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base SystemTree
*/

#pragma once

#include "Systems/ISystem.hpp"
#include <list>


    namespace ECS::Systems
    {
        enum PureSystemGroups {
            NONESYSGROUP = 0,
            ROOTSYSGROUP = 1,
            SYNCSYSGROUP = 2,
        };

        class SystemTreeNode
        {
        public:
            explicit SystemTreeNode(int group, std::list<ISystem *> startSystems = std::list<ISystem *>(), std::list<ISystem *> endSystems = std::list<ISystem *>(), std::list<SystemTreeNode> children = std::list<SystemTreeNode>());
            ~SystemTreeNode() = default;
            SystemTreeNode(const SystemTreeNode &other) = delete;
            SystemTreeNode(SystemTreeNode &&other) = default;
            SystemTreeNode &operator=(const SystemTreeNode &other) = delete;
            SystemTreeNode &operator=(SystemTreeNode &&other) = delete;
            bool addSystemGroup(int group, int neighbourGroup, bool addBefore, bool addInside);
            bool addSystem(ISystem *system, int group, bool atStart);
            void registerEntityPool(Entities::IEntityPool *entityPool);
            void runNode();
            [[nodiscard]] int getGroup() const;
        private:
            int _group;

            std::list<ISystem *> _startSystems;
            std::list<SystemTreeNode> _children;
            std::list<ISystem *> _endSystems;
        };


        class SystemTree
        {
        public:
            SystemTree();
            ~SystemTree();
            SystemTree(const SystemTree &other) = delete;
            SystemTree(SystemTree &&other) = delete;
            SystemTree &operator=(const SystemTree &other) = delete;
            SystemTree &operator=(SystemTree &&other) = delete;
            bool addSystemGroup(int group, int neighbourGroup, bool addBefore, bool addInside);
            bool addSystem(ISystem *system, int group, bool atStart);
            void registerEntityPool(Entities::IEntityPool *entityPool);
            void runTree();
        private:
            SystemTreeNode _root;
        };
    }
