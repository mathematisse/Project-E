
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
    client.connect_tcp("127.0.0.1", port);
    while (true) {
        client.update();
    }
    return 0;
}
