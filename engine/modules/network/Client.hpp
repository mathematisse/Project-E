#pragma once

#include "lib_net/BaseClient.hpp"
#include "lib_net/Packet.hpp"
#include <cstdint>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#else
#include <ncurses.h>
#endif

namespace network {
enum class CustomMsgTypes : uint32_t {
    ServerAccept,
    ServerDeny,
    ServerPing,
    MessageAll,
    ServerMessage,
};

class CustomClient : public net::BaseClient<CustomMsgTypes> {
public:
    void PingServer()
    {
        net::Packet<CustomMsgTypes> msg;
        msg.header.type = CustomMsgTypes::ServerPing;

        // Caution with this...
        std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();

        msg << timeNow;
        send(msg);
    }

    void MessageAll()
    {
        net::Packet<CustomMsgTypes> msg;
        msg.header.type = CustomMsgTypes::MessageAll;
        send(msg);
    }
};

}
