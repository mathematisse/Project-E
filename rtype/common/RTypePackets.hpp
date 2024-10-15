/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** oui
*/

#pragma once
#include "lib_net/Packet.hpp"

namespace ECS {

enum RTypePacketType : net::Packet::MsgType {
    PLAYER_VELOCITY = 256,
    NEW_ENNEMY = 257,
    BULLET_SHOT = 258,
    ENTITY_DESTROYED = 259,
    PLAYER_CONNECTION_SUCCESS = 260,
    PLAYER_STATE = 261,
    FRAME_ID = 262,
    PLAYER_HURT = 263,
};

// from client to server
struct PlayerVelocityInput {
    float x;
    float y;
    bool shoot;
};

// from server to client
struct PlayerState {
    float x;
    float y;
    float vx;
    float vy;
    size_t health;
    size_t netId;
};

struct NewEnnemy {
    float x;
    float y;
    size_t netId;
};

struct BulletShot {
    float x;
    float y;
    bool isPlayer;
    size_t netId;
};

struct EntityDestroyed {
    size_t netId;
};

struct PlayerConnectionSuccess {
    size_t netId;
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

struct FrameId {
    size_t frame;
};

} // namespace ECS