#include "TestServer.hpp"
#include "lib_net/Packet.hpp"

void net::TestServer::on_tcp_connect(client_id id)
{
    std::cout << "Client connected: " << id << std::endl;
    send_tcp(id, 255, {'h', 'e', 'l', 'l', 'o'});
    // get the generated number from the client
    try {
        auto number = clients.at(id).generated_number;
        auto number_interpreted_as_vector = std::vector<std::uint8_t>(
            reinterpret_cast<std::uint8_t *>(&number),
            reinterpret_cast<std::uint8_t *>(&number) + sizeof(number)
        );
        send_tcp(id, net::Packet::ASKUDP_NUMBER, number_interpreted_as_vector);
    } catch (const std::out_of_range &e) {
        std::cerr << "Client not found: " << id << std::endl;
    }
}

void net::TestServer::on_udp_connect(client_id id)
{
    std::cout << "UDP client connected: " << id << std::endl;
}

void net::TestServer::on_tcp_disconnect(client_id id)
{
    std::cout << "Client disconnected: " << id << std::endl;
}

void net::TestServer::on_packet(const Packet &packet, client_id id)
{
    std::cout << "Received packet from client " << id << std::endl;
    std::cout << "Packet size: " << packet.header.size << std::endl;
    if (packet.header.size == 0) {
        return;
    }
    std::cout << "Packet data: ";
    // convert to string
    std::string data(packet.data.begin(), packet.data.end());
    std::cout << "\"" << data << "\"" << std::endl;
}
