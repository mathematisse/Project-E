
#include "World.hpp"
#include "raylib.h"
#include "utils/debug.hpp"
#include <cassert>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <limits>
#include <thread>

struct CPosition {
    float x, y;

    DERIVE_DEBUG(CPosition, x, y)
};

struct CVelocity {
    float x, y;

    DERIVE_DEBUG(CVelocity, x, y)
};

struct CCollider {
    enum class Type : uint8_t {
        Deflecting,
        Push,
        Slide,
    };

    Type type;
};

// AABB collision box
struct CCollision {
    size_t entity;

    DERIVE_DEBUG(CCollision, entity)
};

struct CSpeed {
    float horizontal;

    DERIVE_DEBUG(CSpeed, horizontal)
};

struct CRectangle {
    float width, height;

    DERIVE_DEBUG(CRectangle, width, height)
};

struct CColor {
    uint8_t r, g, b, a;

    CColor() = default;
    CColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a):
        r(r),
        g(g),
        b(b),
        a(a)
    {
    }
    CColor(const Color &color):
        r(color.r),
        g(color.g),
        b(color.b),
        a(color.a)
    {
    }

    // automatic conversion to Color from raylib
    operator Color() const { return {r, g, b, a}; }
    DERIVE_DEBUG(CColor, r, g, b, a)
};

struct CInput {
public:
    // bitfield
    enum class Key : uint8_t {
        None = 0,
        Up = 1 << 1,
        Down = 1 << 2,
        Left = 1 << 3,
        Right = 1 << 4,
        Fire = 1 << 5,
        Spawn = 1 << 6,
    };

    Key key = Key::None;

    // automatic conversion to uint8_t
    operator uint8_t() const { return static_cast<uint8_t>(key); }
    operator bool() const { return key != Key::None; }

    auto operator&(Key other) const -> bool
    {
        return static_cast<uint8_t>(key) & static_cast<uint8_t>(other);
    }

    auto operator|=(Key other) -> CInput &
    {
        key = static_cast<Key>(static_cast<uint8_t>(key) | static_cast<uint8_t>(other));
        return *this;
    }
};

struct CPlayer { };

template<class World>
void Srectangle_draw(World &world)
{
    for (auto entity : world.template view<CPosition, CRectangle, CColor>()) {
        if (auto opt = world.template get<CPosition, CRectangle, CColor>(entity); opt.has_value()) {
            auto &[pos, rect, color] = opt.value();
            DrawRectangle(
                static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(rect.width),
                static_cast<int>(rect.height), static_cast<Color>(color)
            );
        }
    }
}

// print debug information of the player structs with raylib
template<class World>
void Splayer_draw_debug(World &world)
{
    for (auto entity : world.template view<CPlayer, CPosition, CVelocity>()) {
        if (auto opt = world.template get<CPlayer, CPosition, CVelocity>(entity); opt.has_value()) {
            auto &[_, pos, velocity] = opt.value();
            auto collision = world.template has<CCollision>(entity);
            DrawText(
                std::string(
                    "Player: x: " + std::to_string(pos.x) + "\ny: " + std::to_string(pos.y) +
                    "\nvx: " + std::to_string(velocity.x) + "\nvy: " + std::to_string(velocity.y) +
                    "\ncolliding: " + (collision ? "true" : "false")
                )
                    .c_str(),
                10, 10, 16, GREEN
            );
        }
    }
}

template<class World>
void Sgravity_update(World &world, float dt)
{
    for (auto entity : world.template view<CVelocity>()) {
        if (auto opt = world.template get<CVelocity>(entity); opt.has_value()) {
            auto &[velocity] = opt.value();
            const float GRAVITY = 9.8f;
            velocity.y += GRAVITY * dt;
        }
    }
}

template<class World>
void Smovement_update(World &world, float dt)
{
    for (auto entity : world.template view<CVelocity, CPosition, CSpeed>()) {
        if (auto opt = world.template get<CVelocity, CPosition, CSpeed>(entity); opt.has_value()) {
            auto &[velocity, pos, speed] = opt.value();
            auto notplayer = !world.template has<CPlayer>(entity);

            if (notplayer) {
                std::cout << "Position: " << pos << std::endl;
            }
            pos.x += velocity.x * speed.horizontal * dt;
            pos.y += velocity.y * speed.horizontal * dt;
            if (notplayer) {
                std::cout << "NEW Position: " << pos << ", " << velocity << ", " << speed << ", " << dt
                          << std::endl;
            }
        }
    }
}

// Swept AABB Collision Detection
// https://www.gamedev.net/tutorials/programming/general-and-gameplay-programming/swept-aabb-collision-detection-and-response-r3084/
float swept_aabb(
    CPosition &pos1, CRectangle &rect1, CVelocity &vel1, CPosition &pos2, CRectangle &rect2, CVelocity &vel2,
    float &normalx, float &normaly
)
{
    float xInvEntry, yInvEntry;
    float xInvExit, yInvExit;

    // find the distance between the objects on the near and far sides for both x and y
    if (vel1.x > 0.0f) {
        xInvEntry = pos2.x - (pos1.x + rect1.width);
        xInvExit = (pos2.x + rect2.width) - pos1.x;
    } else {
        xInvEntry = (pos2.x + rect2.width) - pos1.x;
        xInvExit = pos2.x - (pos1.x + rect1.width);
    }

    if (vel1.y > 0.0f) {
        yInvEntry = pos2.y - (pos1.y + rect1.height);
        yInvExit = (pos2.y + rect2.height) - pos1.y;
    } else {
        yInvEntry = (pos2.y + rect2.height) - pos1.y;
        yInvExit = pos2.y - (pos1.y + rect1.height);
    }

    // find time of collision and time of leaving for each axis (if statement is to prevent divide by zero)
    float xEntry = 0.0f, yEntry = 0.0f;
    float xExit = 0.0f, yExit = 0.0f;

    if (vel1.x == 0.0f) {
        xEntry = -std::numeric_limits<float>::infinity();
        xExit = std::numeric_limits<float>::infinity();
    } else {
        xEntry = xInvEntry / vel1.x;
        xExit = xInvExit / vel1.x;
    }

    if (vel1.y == 0.0f) {
        yEntry = -std::numeric_limits<float>::infinity();
        yExit = std::numeric_limits<float>::infinity();
    } else {
        yEntry = yInvEntry / vel1.y;
        yExit = yInvExit / vel1.y;
    }

    // find the earliest/latest times of collision
    float entryTime = std::max(xEntry, yEntry);
    float exitTime = std::min(xExit, yExit);

    // if there was no collision
    if (entryTime > exitTime || xEntry < 0.0f && yEntry < 0.0f || xEntry > 1.0f || yEntry > 1.0f) {
        normalx = 0.0f;
        normaly = 0.0f;
        return 1.0f;
    } else {
        // calculate normal of collided surface
        if (xEntry > yEntry) {
            if (xInvEntry < 0.0f) {
                normalx = 1.0f;
                normaly = 0.0f;
            } else {
                normalx = -1.0f;
                normaly = 0.0f;
            }
        } else {
            if (yInvEntry < 0.0f) {
                normalx = 0.0f;
                normaly = 1.0f;
            } else {
                normalx = 0.0f;
                normaly = -1.0f;
            }
        }

        return entryTime;
    }
}

template<class World>
void Scollision_update(World &world, float dt)
{
    // AABB collision detection
    for (auto entity : world.template view<CPosition, CRectangle, CCollider, CVelocity>()) {
        if (auto opt = world.template get<CPosition, CRectangle, CCollider, CVelocity>(entity);
            opt.has_value()) {
            auto &[pos, rect, collision, velocity] = opt.value();
            auto nearest_collision = std::numeric_limits<uint32_t>::max();
            world.template remove<CCollision>(entity);
            for (auto other : world.template view<CPosition, CRectangle, CCollider>()) {
                if (entity == other) {
                    continue;
                }
                auto opt_other = world.template get<CPosition, CRectangle, CCollider>(other);
                // safe to unwrap since we know the entity exists, since we are iterating over it
                auto &[pos_other, rect_other, collision_other] = opt_other.value();
                // auto collision_distance = std::max(
                //     std::max(pos.x - (pos_other.x + rect_other.width), pos_other.x - (pos.x + rect.width)),
                //     std::max(pos.y - (pos_other.y + rect_other.height), pos_other.y - (pos.y +
                //     rect.height))
                // );
                if (pos.x < pos_other.x + rect_other.width && pos.x + rect.width > pos_other.x &&
                    pos.y < pos_other.y + rect_other.height && pos.y + rect.height > pos_other.y) {
                    // Only supports 1 collision at a time (should take the nearest collision)
                    world.template add<CCollision>(entity, CCollision {other});
                }
            }
        }
    }

    // Swept AABB collision detection
    for (auto entity : world.template view<CCollision, CVelocity, CPosition, CSpeed, CRectangle>()) {
        if (auto opt = world.template get<CCollision, CVelocity, CPosition, CSpeed, CRectangle>(entity);
            opt.has_value()) {
            auto &[collision, velocity, position, speed, rect] = opt.value();
            auto opt_other = world.template get<CCollider, CPosition, CRectangle>(collision.entity);
            // safe to unwrap since temporary Component CCollision contains the entity id and we destroy
            // entities at the end
            auto &[collision_other, position_other, rect_other] = opt_other.value();
            float normalx;
            float normaly;
            CVelocity velocity_other {0, 0};
            auto collision_time = swept_aabb(
                position, rect, velocity, position_other, rect_other, velocity_other, normalx, normaly
            );
            position.x -= velocity.x * speed.horizontal * collision_time * dt;
            position.y -= velocity.y * speed.horizontal * collision_time * dt;
            if (!world.template has<CPlayer>(entity)) {
                std::cout << "Collision time: " << collision_time << std::endl;
                std::cout << "New Position: " << position << std::endl;
            }
            float remaining_time = 1.0f - collision_time;
            // slide
            float dotprod = (velocity.x * normaly + velocity.y * normalx) * remaining_time;
            velocity.x = dotprod * normaly;
            velocity.y = dotprod * normalx;
        }
    }
}

template<class World>
void Splayer_rectangle_update(World &world)
{
    for (auto entity : world.template view<CPlayer, CPosition, CRectangle, CColor>()) {
        if (auto opt = world.template get<CPlayer, CPosition, CRectangle, CColor>(entity); opt.has_value()) {
            auto &[_, pos, rect, color] = opt.value();
            if (world.template has<CCollision>(entity)) {
                color = CColor {0, 255, 0, 255};
            } else {
                color = CColor {255, 0, 0, 255};
            }
        }
    }
}

template<class World>
void Sinput_get(World &world)
{
    for (auto entity : world.template view<CInput, CPlayer>()) {
        if (auto opt = world.template get<CInput, CPlayer>(entity); opt.has_value()) {
            auto &[input, _] = opt.value();
            input.key = CInput::Key::None;
            if (IsKeyDown(KEY_UP)) {
                input |= CInput::Key::Up;
            }
            if (IsKeyDown(KEY_DOWN)) {
                input |= CInput::Key::Down;
            }
            if (IsKeyDown(KEY_LEFT)) {
                input |= CInput::Key::Left;
            }
            if (IsKeyDown(KEY_RIGHT)) {
                input |= CInput::Key::Right;
            }
            if (IsKeyDown(KEY_SPACE)) {
                input |= CInput::Key::Fire;
            }
            // place a new entity with rectangle on click
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                input |= CInput::Key::Spawn;
            }
        }
    }
}

template<class World>
void Splayer_update_direction(World &world)
{
    for (auto entity : world.template view<CInput, CVelocity>()) {
        if (auto opt = world.template get<CInput, CVelocity>(entity); opt.has_value()) {
            auto &[input, velocity] = opt.value();
            velocity.x = 0;
            velocity.y = 0;
            if (input & CInput::Key::Up) {
                velocity.y = -1;
            }
            if (input & CInput::Key::Down) {
                velocity.y = 1;
            }
            if (input & CInput::Key::Left) {
                velocity.x = -1;
            }
            if (input & CInput::Key::Right) {
                velocity.x = 1;
            }
        }
    }
}

template<class World>
void Splayer_SpawnEntity(World &world, float dt)
{
    static float spawn_cooldown = 0;

    for (auto entity : world.template view<CInput>()) {
        if (auto opt = world.template get<CInput>(entity); opt.has_value()) {
            auto &[input] = opt.value();
            if (input & CInput::Key::Spawn) {
                if (spawn_cooldown > 0) {
                    spawn_cooldown -= dt;
                    continue;
                } else {
                    spawn_cooldown = 1000;
                }
                auto new_entity = world.new_entity();
                std::cout << "\nNew entity: " << new_entity << "\n" << std::endl;
                world.template add<CPosition>(
                    new_entity, CPosition {static_cast<float>(GetMouseX()), static_cast<float>(GetMouseY())}
                );
                world.template add<CRectangle>(new_entity, CRectangle {40, 40});
                world.template add<CColor>(new_entity, CColor {255, 0, 0, 255});
                world.template add<CCollider>(new_entity, CCollider {});
                world.template add<CSpeed>(new_entity, CSpeed {100});
                world.template add<CVelocity>(
                    new_entity,
                    CVelocity {std::numeric_limits<float>::epsilon(), std::numeric_limits<float>::epsilon()}
                );
            }
        }
    }
}

template<class World>
void init_entities(World &world)
{
    auto player = world.new_entity();
    world.template add<CPosition>(player, CPosition {400, 300});
    world.template add<CRectangle>(player, CRectangle {40, 40});
    world.template add<CColor>(player, CColor {255, 0, 0, 255});
    world.template add<CInput>(player, CInput {CInput::Key::None});
    world.template add<CCollider>(player, CCollider {});
    world.template add<CSpeed>(player, CSpeed {100});
    world.template add<CVelocity>(player, CVelocity {0, 0});
    world.template add<CPlayer>(player, CPlayer {});

    auto floor = world.new_entity(); // bottom floor
    world.template add<CPosition>(floor, CPosition {0, 500});
    world.template add<CRectangle>(floor, CRectangle {800, 100});
    world.template add<CColor>(floor, CColor {0, 0, 255, 255});
    world.template add<CCollider>(floor, CCollider {});

    auto wall = world.new_entity(); // top wall
    world.template add<CPosition>(wall, CPosition {0, 0});
    world.template add<CRectangle>(wall, CRectangle {800, 100});
    world.template add<CColor>(wall, CColor {0, 0, 255, 255});
    world.template add<CCollider>(wall, CCollider {});

    // ball that bounces up and down
    auto ball = world.new_entity();
    world.template add<CPosition>(ball, CPosition {400, 100});
    world.template add<CRectangle>(ball, CRectangle {20, 20});
    world.template add<CColor>(ball, CColor {0, 25, 178, 255});
    world.template add<CCollider>(ball, CCollider {});
    world.template add<CSpeed>(ball, CSpeed {100});
    world.template add<CVelocity>(ball, CVelocity {0, 1});
}

int main()
{
    World<CPosition, CRectangle, CColor, CInput, CCollision, CCollider, CSpeed, CVelocity, CPlayer> world;

    init_entities(world);
    InitWindow(800, 600, "ECS Test");

    SetTargetFPS(60);

    auto curr_time = std::chrono::steady_clock::now();
    while (!WindowShouldClose()) {
        auto new_time = std::chrono::steady_clock::now();
        auto dt = std::chrono::duration<float>(new_time - curr_time).count();
        curr_time = new_time;

        Sgravity_update(world, dt);
        Sinput_get(world);
        Splayer_SpawnEntity(world, dt);
        Splayer_update_direction(world);
        Smovement_update(world, dt);
        Scollision_update(world, dt);
        Splayer_rectangle_update(world);
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            Srectangle_draw(world);
            Splayer_draw_debug(world);
        }
        EndDrawing();
        // sleep for 16ms
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    return 0;
}
