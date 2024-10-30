#include <algorithm>
#include <iostream>
#include <optional>
#include <vector>
#include "lib_net/net/IpAddr.hpp"
#include "lib_net/net/TcpListener.hpp"
#include "lib_net/net/TcpStream.hpp"
#include "lib_net/net/UdpSocket.hpp"
#include "lib_net/net/Poll.hpp"

// very uggly example, but shows how to use the library
int main()
{
    try {
        using namespace lnet;
        net::Poll poll;

        // Create and configure TcpListener
        auto tcp_listener = net::TcpListener::bind(net::SocketAddr(net::Ipv4Addr(0), 8080));
        if (!tcp_listener) {
            std::cerr << "TcpListener bind error: " << tcp_listener.error().message() << std::endl;
            return 1;
        }
        auto listen_result = tcp_listener.value();

        std::cout << "TcpListener bound to: " << listen_result.local_addr().value().to_string()
                  << std::endl;

        // Add TcpListener to Poll for read events

        if (auto res = poll.add(listen_result); !res) {
            auto error = res.error();
            std::cerr << "Poll add_read tcp error: " << error.message() << std::endl;
            return 1;
        }

        // Create and configure UdpSocket
        auto udp_socket = net::UdpSocket::bind(net::SocketAddr(net::Ipv4Addr(0), 8081));

        if (!udp_socket) {
            std::cerr << "UdpSocket bind error: " << udp_socket.error().message() << std::endl;
            return 1;
        }
        auto udp_socket_result = udp_socket.value();

        std::cout << "UdpSocket bound to: "
                  << udp_socket_result._sock.local_addr().value().to_string() << std::endl;
        // Add UdpSocket to Poll for read events
        if (auto res = poll.add(udp_socket_result); !res) {
            std::cerr << "Poll add_read udp error: " << res.error().message() << std::endl;
            return 1;
        }

        std::vector<net::TcpStream> tcp_connections;

        while (true) {
            auto result = poll.wait(std::nullopt);
            if (!result) {
                std::cerr << "Poll wait error: " << result.error().message() << std::endl;
                break;
            }

            for (const auto &event : result.value()) {
                if (event.type == net::PollEvent::Type::Read) {
                    if (event.fd == listen_result.get_fd()) {
                        // Handle new TCP connection
                        auto new_connection = listen_result.accept();
                        if (new_connection) {
                            auto [stream, addr] = new_connection.value();
                            if (!poll.add(stream)) {
                                std::cerr << "Poll add_read error" << std::endl;
                                return 1;
                            }
                            tcp_connections.push_back(std::move(stream));
                            std::cout << "New TCP connection accepted" << std::endl;
                        }
                    } else if (event.fd == udp_socket_result.get_fd()) {
                        // Handle UDP data
                        std::vector<uint8_t> buffer(1024);
                        auto span_buffer = std::span<std::uint8_t>(
                            reinterpret_cast<std::uint8_t *>(buffer.data()), buffer.size()
                        );
                        auto recv_result = udp_socket_result.recv_from(span_buffer);
                        if (recv_result) {
                            if (auto [size, addr] = recv_result.value(); size > 0) {
                                std::cout << "Received UDP data: " << size << " bytes from "
                                          << addr.to_string() << std::endl;
                            } else {
                                // Datagram allows for empty data (here do nothing)
                            }
                        }
                    } else {
                        // Handle TCP data
                        auto it = std::find_if(
                            tcp_connections.begin(), tcp_connections.end(),
                            [&event](const net::TcpStream &stream) {
                                return stream.get_fd() == event.fd;
                            }
                        );
                        if (it != tcp_connections.end()) {
                            std::vector<uint8_t> buffer(1024);
                            auto span_buffer = std::span<std::uint8_t>(
                                reinterpret_cast<std::uint8_t *>(buffer.data()), buffer.size()
                            );
                            auto recv_result = it->read(span_buffer);
                            if (recv_result) {
                                if (recv_result.value() > 0) {
                                    std::cout << "Received TCP data: " << recv_result.value()
                                              << " bytes" << std::endl;
                                } else {
                                    // Connection closed
                                    auto close_result = poll.remove(*it);
                                    if (!close_result) {
                                        std::cerr << "Poll remove_read error: "
                                                  << close_result.error().message() << std::endl;
                                        return 1;
                                    }
                                    tcp_connections.erase(it);
                                    std::cout << "Connection closed" << std::endl;
                                }
                            }
                        }
                    }
                } else if (event.type == net::PollEvent::Type::Error) {
                    std::cerr << "Poll error on fd: " << event.fd << std::endl;
                }
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}