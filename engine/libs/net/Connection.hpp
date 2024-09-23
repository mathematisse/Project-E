
#pragma once

#include "Packet.hpp"
#include "TSQueue.hpp"
#include <iostream>
#include <memory>
#include <boost/asio.hpp>

namespace net {

enum class Ownership {
    Client,
    Server,
};

template<Ownership O, typename T>
class Connection : public std::enable_shared_from_this<Connection<O, T>> {

public:
    Connection(boost::asio::io_context &context, boost::asio::ip::tcp::socket socket, TSQueue<T> &queueIn):
        _context(context),
        _socket(std::move(socket)),
        _queueIn(queueIn)
    {
    }
    Connection(const Connection &) = default;
    Connection(Connection &&) = default;
    Connection &operator=(const Connection &) = default;
    Connection &operator=(Connection &&) = default;

    virtual ~Connection() = default;

    void connect(const boost::asio::ip::tcp::resolver::results_type &endpoints)
    {
        if constexpr (O == Ownership::Client) {
            boost::asio::async_connect(
                _socket, endpoints,
                [this](std::error_code error_code, boost::asio::ip::tcp::endpoint endpoint) {
                    if (!error_code) {
                        ReadHeader();
                    }
                }
            );
        } else {
            static_assert(O == Ownership::Client, "Server connections should not connect to other servers.");
        }
    }

    void connect()
    {
        if constexpr (O == Ownership::Server) {
            if (_socket.is_open()) {
                ReadHeader();
            }
        } else {
            static_assert(O == Ownership::Server, "Client connections should connect to servers.");
        }
    }

    void AddToIncomingMessageQueue()
    {
        if constexpr (O == Ownership::Server) {
            _queueIn.push_back({this->shared_from_this(), _tmpPacket});
        } else {
            _queueIn.push_back({nullptr, _tmpPacket});
        }
        ReadHeader();
    }

    void Disconnect()
    {
        if (IsConnected()) {
            boost::asio::post(_context, [this]() {
                _socket.close();
            });
        }
    }

    [[nodiscard]] bool IsConnected() const { return _socket.is_open(); }

    void StartListening() { }

    void Send(const Packet<T> &msg)
    {
        boost::asio::post(_context, [this, msg]() {
            bool is_writing = !_queueOut.empty();
            _queueOut.push_back(msg);
            if (!is_writing) {
                WriteHeader();
            }
        });
    }

private:
    void WriteHeader()
    {
        boost::asio::async_write(
            _socket, boost::asio::buffer(&_queueOut.front().header, sizeof(Packet<T>::Header)),
            [this](std::error_code error_code, std::size_t length) {
                if (!error_code) {
                    if (_queueOut.front().body.size() > 0) {
                        WriteBody();
                    } else {
                        _queueOut.pop_front();

                        if (!_queueOut.empty()) {
                            WriteHeader();
                        }
                    }
                } else {
                    std::cout << "[] Write Header Fail.\n";
                    _socket.close();
                }
            }
        );
    }

    void WriteBody()
    {
        boost::asio::async_write(
            _socket, boost::asio::buffer(_queueOut.front().body.data(), _queueOut.front().body.size()),
            [this](std::error_code error_code, std::size_t length) {
                if (!error_code) {
                    _queueOut.pop_front();

                    if (!_queueOut.empty()) {
                        WriteHeader();
                    }
                } else {
                    std::cout << "[] Write Body Fail.\n";
                    _socket.close();
                }
            }
        );
    }

    void ReadHeader()
    {
        boost::asio::async_read(
            _socket, boost::asio::buffer(&_queueIn.header, sizeof(Packet<T>::Header)),
            [this](std::error_code error_code, std::size_t length) {
                if (!error_code) {
                    if (_queueIn.header.size > 0) {
                        _queueIn.body.resize(_queueIn.header.size);
                        ReadBody();
                    } else {
                        AddToIncomingMessageQueue();
                    }
                } else {
                    std::cout << "[] Read Header Fail.\n";
                    _socket.close();
                }
            }
        );
    }

    void ReadBody()
    {
        boost::asio::async_read(
            _socket, boost::asio::buffer(_queueIn.body.data(), _queueIn.body.size()),
            [this](std::error_code error_code, std::size_t length) {
                if (!error_code) {
                    AddToIncomingMessageQueue();
                } else {
                    std::cout << "[] Read Body Fail.\n";
                    _socket.close();
                }
            }
        );
    }

protected:
    boost::asio::io_context &_context;
    boost::asio::ip::tcp::socket _socket;
    TSQueue<T> &_queueIn;
    TSQueue<T> _queueOut;
    Packet<T> _tmpPacket;
};

} // namespace net