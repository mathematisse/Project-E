#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <thread>
#include <deque>
#include <algorithm>

#include <boost/asio.hpp>

#include "Connection.hpp"
#include "OwnedPacket.hpp"

namespace net {
template<typename T, Ownership O = Ownership::Server>
class BaseServer {

public:
    explicit BaseServer(uint16_t port):
        m_asioAcceptor(_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
    {
    }
    BaseServer(const BaseServer &) = default;
    BaseServer &operator=(const BaseServer &) = default;
    BaseServer(BaseServer &&) = default;
    BaseServer &operator=(BaseServer &&) = default;

    virtual ~BaseServer() { stop(); }

    bool start()
    {
        try {

            wait_for_client_connection();
            _threadContext = std::thread([this]() {
                _context.run();
            });
        } catch (std::exception &e) {
            std::cerr << "[SERVER] Exception: " << e.what() << "\n";
            return false;
        }

        std::cout << "[SERVER] Started!\n";
        return true;
    }

    void stop()
    {
        _context.stop();

        if (_threadContext.joinable()) {
            _threadContext.join();
        }

        std::cout << "[SERVER] Stopped!\n";
    }

    void wait_for_client_connection()
    {
        m_asioAcceptor.async_accept([this](std::error_code error_code, boost::asio::ip::tcp::socket socket) {
            if (!error_code) {
                std::cout << "[SERVER] New Connection: " << socket.remote_endpoint() << "\n";

                auto newconn = std::make_shared<Connection<O, T>>(_context, std::move(socket), _queueIn);

                if (on_client_connect(newconn)) {
                    _connections.push_back(std::move(newconn));

                    _connections.back()->connect();

                    std::cout << "[] Connection Approved\n";
                } else {
                    std::cout << "[-----] Connection Denied\n";
                }
            } else {
                std::cout << "[SERVER] New Connection Error: " << error_code.message() << "\n";
            }

            wait_for_client_connection();
        });
    }

    void send_client(std::shared_ptr<Connection<O, T>> client, const Packet<T> &packet)
    {
        if (client && client->IsConnected()) {
            client->Send(packet);
        } else {
            on_client_disconnect(client);

            client.reset();

            _connections.erase(
                std::remove(_connections.begin(), _connections.end(), client), _connections.end()
            );
        }
    }

    void send_all_clients(const Packet<T> &packet, std::shared_ptr<Connection<O, T>> pIgnoreClient = nullptr)
    {
        bool bInvalidClientExists = false;

        for (auto &client : _connections) {
            if (client && client->is_connected()) {
                if (client != pIgnoreClient) {
                    client->send(packet);
                }
            } else {
                on_client_disconnect(client);
                client.reset();

                bInvalidClientExists = true;
            }
        }

        if (bInvalidClientExists) {
            _connections.erase(
                std::remove(_connections.begin(), _connections.end(), nullptr), _connections.end()
            );
        }
    }

    void update(size_t nMaxPackets = -1, bool bWait = false)
    {
        if (bWait) {
            _queueIn.wait();
        }

        size_t nPacketsCount = 0;
        while (nPacketsCount < nMaxPackets && !_queueIn.empty()) {
            auto packet = _queueIn.pop_front();

            on_packet(packet.remote, packet.packet);

            nPacketsCount++;
        }
    }

protected:
    virtual bool on_client_connect(std::shared_ptr<Connection<O, T>> /*client*/) { return false; }

    virtual void on_client_disconnect(std::shared_ptr<Connection<O, T>> /*client*/) { }

    virtual void on_packet(std::shared_ptr<Connection<O, T>> /*client*/, Packet<T> & /*packet*/) { }

    TSQueue<OwnedPacket<O, T>> _queueIn;

    std::deque<std::shared_ptr<Connection<O, T>>> _connections;

    boost::asio::io_context _context;
    std::thread _threadContext;

    boost::asio::ip::tcp::acceptor m_asioAcceptor;
};
}
