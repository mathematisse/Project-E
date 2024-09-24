
#include "network/Server.hpp"

int main()
{
    CustomServer server(60000);
    server.start();

    while (true) {
        server.update(-1, true);
    }

    return 0;
}