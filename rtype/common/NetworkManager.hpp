#pragma once

#include "lib_ecs/Core/ComponentMaker.hpp"

DECLARE_COMPONENT(NetworkID, size_t);

class NetworkManager {
public:
    NetworkManager() = default;
    ~NetworkManager() = default;

    void incrementNetID() { netID++; }
    [[nodiscard]] unsigned long getNetID() const { return netID; }
    [[nodiscard]] unsigned long getnewNetID()
    {
        netID++;
        return netID - 1;
    }

private:
    unsigned long netID = 1;
};
