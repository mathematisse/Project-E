#pragma once

#include <memory>
#include <random>
#include <unordered_set>
#include <limits>

#include "lib_net/uuid/Uuid.hpp"

namespace lnet::uuid {

// Make use of std::mt19937_64 to be the generator
template<typename G = std::mt19937_64>
    requires std::is_same_v<G, std::mt19937> || std::is_same_v<G, std::mt19937_64>
class UuidGenerator {
public:
    UuidGenerator():
        generator(new G(std::random_device()())),
        distribution(std::numeric_limits<uint64_t>::min(), std::numeric_limits<uint64_t>::max())
    {
    }

    explicit UuidGenerator(uint64_t seed):
        generator(new G(seed)),
        distribution(std::numeric_limits<uint64_t>::min(), std::numeric_limits<uint64_t>::max())
    {
    }

    explicit UuidGenerator(G &gen):
        generator(gen),
        distribution(std::numeric_limits<uint64_t>::min(), std::numeric_limits<uint64_t>::max())
    {
    }

    Uuid new_uuid()
    {
        uint64_t uuid = 0;
        do {
            uuid = distribution(*generator);
        } while (generatedUUIDs.find(uuid) != generatedUUIDs.end());

        generatedUUIDs.insert(uuid);
        return {uuid};
    }

    void reset() { generatedUUIDs.clear(); }

    void delete_uuid(const Uuid &uuid) { generatedUUIDs.erase(uuid.data); }

private:
    std::shared_ptr<G> generator;
    std::uniform_int_distribution<uint64_t> distribution;
    std::unordered_set<uint64_t> generatedUUIDs;
};

}

namespace std {

template<>
struct hash<lnet::uuid::Uuid> {
    size_t operator()(const lnet::uuid::Uuid &uuid) const { return uuid.hash(); }
};

}