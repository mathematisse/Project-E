/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base Components
*/

#pragma once

#include "Components/IComponent.hpp"

namespace ECS::Components {

/**
 * @brief Base component class template.
 * 
 * @tparam T Type of the component.
 */
template <typename T>
class ComponentRef : public IComponentRef {
public:
    /**
     * @brief Construct a new Component object with a pointer.
     * 
     * @param x Pointer to the component value.
     */
    explicit ComponentRef(T *x) : _x(x), _dummyX(0) {}

    /**
     * @brief Construct a new Component object with a value.
     * 
     * @param x Value of the component.
     */
    explicit ComponentRef(T x) : _x(&_dummyX), _dummyX(x) {}

    /**
     * @brief Get the component value pointer.
     * 
     * @return T* Pointer to the component value.
     */
    T *GetX() { return _x; }

    /**
     * @brief Get the component value pointer (const version).
     * 
     * @return const T* Pointer to the component value.
     */
    const T *GetX() const { return _x; }

    /**
     * @brief Set the component value.
     * 
     * @param x New value of the component.
     */
    void SetX(T x) { *_x = x; }

protected:
    T *_x;       ///< Pointer to the component value.
    T _dummyX;   ///< Dummy value for internal use.
};

/**
 * @brief Component class template with two values.
 * 
 * @tparam T Type of the component.
 */
template <typename T>
class ComponentRef2 : public ComponentRef<T> {
public:
    /**
     * @brief Construct a new Component2 object with pointers.
     * 
     * @param x Pointer to the first component value.
     * @param y Pointer to the second component value.
     */
    ComponentRef2(T *x, T *y) : ComponentRef<T>(x), _y(y), _dummyY(0) {}

    /**
     * @brief Construct a new Component2 object with values.
     * 
     * @param x First component value.
     * @param y Second component value.
     */
    ComponentRef2(T x, T y) : ComponentRef<T>(x), _y(&_dummyY), _dummyY(y) {}

    /**
     * @brief Get the second component value pointer.
     * 
     * @return T* Pointer to the second component value.
     */
    T *GetY() { return _y; }

    /**
     * @brief Get the second component value pointer (const version).
     * 
     * @return const T* Pointer to the second component value.
     */
    const T *GetY() const { return _y; }

    /**
     * @brief Set the second component value.
     * 
     * @param y New value of the second component.
     */
    void SetY(T y) { *_y = y; }

protected:
    T *_y;       ///< Pointer to the second component value.
    T _dummyY;   ///< Dummy value for internal use.
};

/**
 * @brief Component class template with three values.
 * 
 * @tparam T Type of the component.
 */
template <typename T>
class ComponentRef3 : public ComponentRef2<T> {
public:
    /**
     * @brief Construct a new Component3 object with pointers.
     * 
     * @param x Pointer to the first component value.
     * @param y Pointer to the second component value.
     * @param z Pointer to the third component value.
     */
    ComponentRef3(T *x, T *y, T *z) : ComponentRef2<T>(x, y), _z(z), _dummyZ(0) {}

    /**
     * @brief Construct a new Component3 object with values.
     * 
     * @param x First component value.
     * @param y Second component value.
     * @param z Third component value.
     */
    ComponentRef3(T x, T y, T z) : ComponentRef2<T>(x, y), _z(&_dummyZ), _dummyZ(z) {}

    /**
     * @brief Get the third component value pointer.
     * 
     * @return T* Pointer to the third component value.
     */
    T *GetZ() { return _z; }

    /**
     * @brief Get the third component value pointer (const version).
     * 
     * @return const T* Pointer to the third component value.
     */
    const T *GetZ() const { return _z; }

    /**
     * @brief Set the third component value.
     * 
     * @param z New value of the third component.
     */
    void SetZ(T z) { *_z = z; }

protected:
    T *_z;       ///< Pointer to the third component value.
    T _dummyZ;   ///< Dummy value for internal use.
};

/**
 * @brief Component class template with four values.
 * 
 * @tparam T Type of the component.
 */
template <typename T>
class ComponentRef4 : public ComponentRef3<T> {
public:
    /**
     * @brief Construct a new Component4 object with pointers.
     * 
     * @param x Pointer to the first component value.
     * @param y Pointer to the second component value.
     * @param z Pointer to the third component value.
     * @param w Pointer to the fourth component value.
     */
    ComponentRef4(T *x, T *y, T *z, T *w) : ComponentRef3<T>(x, y, z), _w(w), _dummyW(0) {}

    /**
     * @brief Construct a new Component4 object with values.
     * 
     * @param x First component value.
     * @param y Second component value.
     * @param z Third component value.
     * @param w Fourth component value.
     */
    ComponentRef4(T x, T y, T z, T w) : ComponentRef3<T>(x, y, z), _w(&_dummyW), _dummyW(w) {}

    /**
     * @brief Get the fourth component value pointer.
     * 
     * @return T* Pointer to the fourth component value.
     */
    T *GetW() { return _w; }

    /**
     * @brief Get the fourth component value pointer (const version).
     * 
     * @return const T* Pointer to the fourth component value.
     */
    const T *GetW() const { return _w; }

    /**
     * @brief Set the fourth component value.
     * 
     * @param w New value of the fourth component.
     */
    void SetW(T w) { *_w = w; }

protected:
    T *_w;       ///< Pointer to the fourth component value.
    T _dummyW;   ///< Dummy value for internal use.
};

}
