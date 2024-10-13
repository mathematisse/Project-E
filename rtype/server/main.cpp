
#include "network/TestServer.hpp"

int main(int ac, char **av)
{
    net::TestServer server;
    std::uint16_t port = 0;

    if (ac != 2) {
        std::cerr << "Usage: ./rtype_server port" << std::endl;
        return 1;
    }
    port = std::stoi(av[1]);

    server.host(port);
    server.start();
    std::cout << "Server started on port " << port << std::endl;
    while (true) {
        // sleep 1 second
        std::this_thread::sleep_for(std::chrono::seconds(1));
        server.update();
    }
    return 0;
}