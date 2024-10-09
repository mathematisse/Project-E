#include "TestServer.hpp"

void net::TestServer::on_connect(client_id id)
{
    std::cout << "Client connected: " << id << std::endl;
    send_tcp(id, 0, {'h', 'e', 'l', 'l', 'o'});
}

void net::TestServer::on_disconnect(client_id id) { std::cout << "Client disconnected: " << id << std::endl; }

void net::TestServer::on_packet(Packet &packet, client_id id)
{
    std::cout << "Received packet from client " << id << std::endl;
    std::cout << "Packet size: " << packet.header.size << std::endl;
    std::cout << "Packet data: ";
    for (auto &byte : packet.data) {
        std::cout << static_cast<char>(byte);
    }
    std::cout << std::endl;
}
