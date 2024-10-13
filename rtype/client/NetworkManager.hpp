#pragma once

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
    unsigned long netID = 0;
};
