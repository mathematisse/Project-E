
#include "network/TestClient.hpp"

int main(int ac, char **av)
{
    net::TestClient client;
    std::uint16_t port = 0;

    if (ac != 2) {
        std::cerr << "Usage: ./rtype_client port" << std::endl;
        return 1;
    }
    port = std::stoi(av[1]);

    std::cout << "Connecting to server on port " << port << std::endl;
    if (client.connect_tcp("127.0.0.1", port)) {
        std::cout << "Connected to server" << std::endl;
    } else {
        std::cerr << "Failed to connect to server" << std::endl;
        return 1;
    }
    if (client.establish_udp_connection("127.0.0.1", port)) {
        std::cout << "Set up UDP Socket" << std::endl;
    } else {
        std::cerr << "Failed to set up UDP Socket" << std::endl;
        return 1;
    }
    while (true) {
        client.update();
    }
    return 0;
}
