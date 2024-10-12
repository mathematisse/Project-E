#include "TestClient.hpp"
#include "lib_net/Packet.hpp"

void net::TestClient::on_packet(const Packet &packet)
{
    std::uint64_t transformed_number;
    std::vector<std::uint8_t> transformed_number_vector;
    std::uint64_t number;

    switch (packet.header.type) {
    case Packet::ASKUDP_NUMBER:
        std::cout << "Received generated number from server: ";
        number = *reinterpret_cast<const std::uint64_t *>(packet.data.data());
        std::cout << number << std::endl;
        // send the number back to the server with the function transformNumberFunction applied
        transformed_number = Gateway::transformNumberFunction(number);
        transformed_number_vector = std::vector<std::uint8_t>(
            reinterpret_cast<std::uint8_t *>(&transformed_number),
            reinterpret_cast<std::uint8_t *>(&transformed_number) + sizeof(transformed_number)
        );
        send_udp(Packet::ASKUDP_RESPONSE, transformed_number_vector);
        break;
    case Packet::PING:
        std::cout << "Received PING from server" << std::endl;
        send_udp(Packet::PONG, {});
        break;
    default:
        std::cout << "Received packet from server" << std::endl;
        std::cout << "Packet size: " << packet.header.size << std::endl;
        if (packet.header.size == 0) {
            return;
        }
        std::cout << "Packet data: ";
        // convert to string
        std::string data(packet.data.begin(), packet.data.end());
        std::cout << "\"" << data << "\"" << std::endl;
    }
}
