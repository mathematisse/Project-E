#ifdef TEST
#include <algorithm>
#include <chrono> // For std::chrono
#include <cstdlib>
#include <iostream> // For std::cout, std::endl
#include <memory>

#include "ComponentStatus.hpp"
#include "World.hpp"

struct Position {
    float x;
    float y;
    float z;

    friend std::ostream &operator<<(std::ostream &os, const Position &pos)
    {
        os << "Position: (" << pos.x << ", " << pos.y << ", " << pos.z << ")";
        return os;
    }
};
struct Level {
    int value;
};
struct C { };
struct D { };
struct E { };
struct F { };
struct G { };
struct H { };
struct I { };

// Helper function to mesure time taken in microseconds, between 2 lines of code
// Usage:
// auto time = measure([]() {
//     // Code to measure
// });
template<typename F>
auto measure(F &&f)
{
    auto start = std::chrono::high_resolution_clock::now();
    f();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

int main()
{
    World<int, Position, Level, D, E, F, G, H, std::unique_ptr<I>> world;

    // Add data to the world
    auto entity = world.new_entity();
    world.add<int>(entity, 42);
    auto i = std::make_unique<I>();
    world.add<std::unique_ptr<I>>(entity, std::move(i));

    if (auto has_data = world.get<int>(entity); has_data.has_value()) {
        auto &[data] = has_data.value();
        // std::cout << "Data found: " << data << std::endl;
    }

    // Create 800 entities
    {
        constexpr size_t num_entities = 8000;
        auto time = measure([&world]() {
            for (size_t i = 0; i < num_entities; ++i) {
                world.new_entity();
            }
        });

        std::cerr << "Time taken to create " << num_entities << " entities: " << time << " nanoseconds"
                  << std::endl;
    }
    // create a World::View for Position
    {
        auto time = measure([&world]() {
            for (auto entityID : world) {
                world.add<Level>(entityID, Level {rand() % 10});
                if (entityID % 2 == 0) {
                    world.add<int>(entityID, rand() % 100);
                }
                if (entityID % 99 == 0) {
                    world.add<Position>(entityID, Position {static_cast<float>(entityID), 0, 0});
                }
            }
        });
        std::cerr << "Time taken to add components to entities: " << time << " nanoseconds" << std::endl;
    }

    {
        auto time = measure([&world]() {
            for (auto entityID : world.view<Position>()) {
                if (auto has_data = world.get<Position>(entityID); has_data.has_value()) {
                    auto &[data] = has_data.value();
                    // std::cout << "Position found: " << data << std::endl;
                }
            }
        });
        std::cerr << "Time taken to iterate over Position components: " << time << " nanoseconds"
                  << std::endl;
    }
    {
        auto time = measure([&world]() {
            for (auto entityID : world.view<Position, int>()) {
                if (auto has_data = world.get<Position, int>(entityID); has_data.has_value()) {
                    auto &[pos, data] = has_data.value();
                    // std::cout << "Position: " << pos << ", Data: " << data << std::endl;
                } else {
                    // std::cout << "No data found for entity " << entityID << std::endl;
                }
            }
        });
        std::cerr << "Time taken to iterate over Position and int components: " << time << " nanoseconds"
                  << std::endl;
    }

    {
        auto time = measure([&world]() {
            std::for_each(world.view<Level>().begin(), world.view<Level>().end(), [&world](auto entityID) {
                if (auto has_data = world.get<Level>(entityID); has_data.has_value()) {
                    auto &[data] = has_data.value();
                    data.value += 10;
                }
            });
        });
        std::cerr << "Time taken to iterate over Level components and editing value: " << time
                  << " nanoseconds" << std::endl;
    }
    {
        auto time = measure([&world]() {
            for (auto entityID : world.view<Level>()) {
                if (auto has_data = world.get<Level>(entityID); has_data.has_value()) {
                    auto &[data] = has_data.value();
                    // std::cout << "Level found: " << data.value << std::endl;
                    world.delete_entity(entityID);
                }
            }
        });
        std::cerr << "Time taken to remove all Entities with Level component: " << time << " nanoseconds"
                  << std::endl;
    }

    for (auto entityID : world.view<Level>()) {
        if (auto has_data = world.get<Level>(entityID); has_data.has_value()) {
            auto &[data] = has_data.value();
            // std::cout << "Level found: " << data.value << std::endl;
        }
    }
    return 0;
}

#endif // TEST