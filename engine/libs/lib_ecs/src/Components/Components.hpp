/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base Components
*/

#pragma once

#include "Components/IComponent.hpp"
#include <iostream>

    namespace ECS::Components
    {
        template <typename T>
        class Component : public IComponent {
        public:
            explicit Component(T *x) : _x(x), _dummyX(0) {
                std::cout << "Component" << std::endl;
                std::cout << "adress value: -" << _x << "-" << std::endl;
                if (x == nullptr)
                    std::cout << "value: " << "null" << std::endl;
                else
                    std::cout << "value: " << *x << std::endl;
            }
            explicit Component(T x) : _x(&_dummyX), _dummyX(x) {}
            T *GetX() { return _x; }
            const T *GetX() const { return _x; }
            void SetX(T x) { 
                std::cout << "SetX" << std::endl;
                std::cout << x << std::endl;
                std::cout << "adress value: " << _x << std::endl;
                if (_x == nullptr)
                    std::cout << "value: " << "null" << std::endl;
                else
                    std::cout << "value: " << *_x << std::endl;
                *_x = x; 
                }
        protected:
            T *_x;
            T _dummyX;
        };

        template <typename T>
        class Component2 : public Component<T> {
        public:
            Component2(T *x, T *y) : Component<T>(x), _y(y), _dummyY(0) {}
            Component2(T x, T y) : Component<T>(x), _y(&_dummyY), _dummyY(y) {}
            T *GetY() { return _y; }
            const T *GetY() const { return _y; }
            void SetY(T y) { *_y = y; }
        protected:
            T *_y;
            T _dummyY;
        };

        template <typename T>
        class Component3 : public Component2<T> {
        public:
            Component3(T *x, T *y, T *z) : Component2<T>(x, y), _z(z), _dummyZ(0) {}
            Component3(T x, T y, T z) : Component2<T>(x, y), _z(&_dummyZ), _dummyZ(z) {}
            T *GetZ() { return _z; }
            const T *GetZ() const { return _z; }
            void SetZ(T z) { *_z = z; }
        protected:
            T *_z;
            T _dummyZ;
        };

        template <typename T>
        class Component4 : public Component3<T> {
        public:
            Component4(T *x, T *y, T *z, T *w) : Component3<T>(x, y, z), _w(w), _dummyW(0) {}
            Component4(T x, T y, T z, T w) : Component3<T>(x, y, z), _w(&_dummyW), _dummyW(w) {}
            T *GetW() { return _w; }
            const T *GetW() const { return _w; }
            void SetW(T w) { *_w = w; }
        protected:
            T *_w;
            T _dummyW;
        };
    }

