
#include "network/TestServer.hpp"

int main()
{
    net::TestServer server;
    std::uint16_t port = 4242;

    server.host(port);
    std::cout << "Server started on port " << port << std::endl;
    while (true) {
        server.update();
    }
    return 0;
}