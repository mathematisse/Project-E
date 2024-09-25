#pragma once

#include <boost/asio.hpp>
#include <memory>

#include "Connection.hpp"
#include "lib_net/OwnedPacket.hpp"
#include "Packet.hpp"
#include "TSQueue.hpp"

namespace net {
template<typename T, Ownership O = Ownership::Client>
class BaseClient {
public:
    BaseClient() = default;
    BaseClient(const BaseClient &) = default;
    BaseClient &operator=(const BaseClient &) = default;
    BaseClient(BaseClient &&) = default;
    BaseClient &operator=(BaseClient &&) = default;

    virtual ~BaseClient() { disconnect(); }

    bool connect(const std::string &host, const uint16_t port)
    {
        try {
            boost::asio::ip::tcp::resolver resolver(_context);
            boost::asio::ip::tcp::resolver::results_type endpoints =
                resolver.resolve(host, std::to_string(port));

            _connection = std::make_unique<Connection<O, T>>(
                _context, boost::asio::ip::tcp::socket(_context), _queueIn
            );

            _connection->connect(endpoints);

            _threadContext = std::thread([this]() {
                _context.run();
            });
        } catch (std::exception &e) {
            std::cerr << "Client Exception: " << e.what() << "\n";
            return false;
        }
        return true;
    }

    void disconnect()
    {
        if (is_connected()) {
            _connection->disconnect();
        }

        _context.stop();
        if (_threadContext.joinable()) {
            _threadContext.join();
        }

        _connection.release();
    }

    bool is_connected()
    {
        if (_connection) {
            return _connection->is_connected();
        } else {
            return false;
        }
    }

    void send(const Packet<T> &packet)
    {
        if (is_connected()) {
            _connection->send(packet);
        }
    }

    TSQueue<OwnedPacket<O, T>> &incoming() { return _queueIn; }

protected:
    boost::asio::io_context _context;
    std::thread _threadContext;
    std::unique_ptr<Connection<O, T>> _connection;

private:
    TSQueue<OwnedPacket<O, T>> _queueIn;
};
}
