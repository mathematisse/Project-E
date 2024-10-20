#include <iostream>
#include <cstring>
#include <thread>
#include <vector>
#include <atomic>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <cassert>
#include <cstdlib>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <chrono>
#define close(s) closesocket(s)
#pragma comment(lib, "Ws2_32.lib")
#else
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#endif

constexpr int MAX_EVENTS = 10;
constexpr int PORT = 8080;
constexpr size_t BUFFER_SIZE = 1024; // Fixed buffer size

// Signal handling
std::atomic<bool> running(true);
int signal_pipe[2];

// Function to handle sending messages to clients
void signal_handler(int signum) {
    // Write to the signal pipe to notify the server thread
    write(signal_pipe[1], &signum, sizeof(signum));
}

// Buffer class for reading data
class ReadBuffer {
public:
    ReadBuffer() : read_pos(0), write_pos(0) {
        buffer.fill(0);
    }

    ssize_t read(int socket) {
        if (write_pos >= BUFFER_SIZE) {
            return 0; // Buffer is full, cannot read more
        }

        ssize_t bytes_read = ::recv(socket, buffer.data() + write_pos, BUFFER_SIZE - write_pos, 0);
        if (bytes_read > 0) {
            write_pos += bytes_read;
        }
        return bytes_read;
    }

    std::string get_data() {
        return std::string(buffer.data(), write_pos);
    }

    void clear() {
        read_pos = 0;
        write_pos = 0;
    }

    void consume(size_t bytes) {
        assert(bytes <= write_pos);
        read_pos += bytes;
        if (read_pos == write_pos) {
            clear(); // Clear buffer if all data consumed
        }
    }

    size_t available() const {
        return write_pos - read_pos;
    }

    const char* current() const {
        return buffer.data() + read_pos;
    }

private:
    std::array<char, BUFFER_SIZE> buffer; // Fixed-size buffer
    size_t read_pos;
    size_t write_pos;
};

// Buffer class for writing data
class WriteBuffer {
public:
    WriteBuffer() : write_pos(0) {
        buffer.fill(0);
    }

    void write(const std::string& data) {
        size_t data_size = data.size();
        if (write_pos + data_size > BUFFER_SIZE) {
            flush(); // Flush if not enough space
        }
        memcpy(buffer.data() + write_pos, data.data(), data_size);
        write_pos += data_size;
    }

    void flush(int socket) {
        if (write_pos > 0) {
            ::send(socket, buffer.data(), write_pos, 0);
            write_pos = 0; // Reset position after flushing
        }
    }

private:
    std::array<char, BUFFER_SIZE> buffer; // Fixed-size buffer
    size_t write_pos;
};

// TCP and UDP server engine
class ServerEngine {
public:
    ServerEngine() {
        // Set up signal handling
        signal(SIGUSR1, signal_handler);

        // Create a pipe for signal communication
        if (pipe(signal_pipe) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        // Create epoll instance
        epoll_fd = epoll_create1(0);
        if (epoll_fd == -1) {
            perror("epoll_create1");
            exit(EXIT_FAILURE);
        }

        // Create TCP socket
        tcp_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (tcp_fd == -1) {
            perror("socket");
            exit(EXIT_FAILURE);
        }

        // Set socket options
        int opt = 1;
        setsockopt(tcp_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        // Bind TCP socket
        sockaddr_in tcp_addr{};
        tcp_addr.sin_family = AF_INET;
        tcp_addr.sin_addr.s_addr = INADDR_ANY;
        tcp_addr.sin_port = htons(PORT);
        if (bind(tcp_fd, (struct sockaddr*)&tcp_addr, sizeof(tcp_addr)) == -1) {
            perror("bind");
            exit(EXIT_FAILURE);
        }

        // Listen on TCP socket
        if (listen(tcp_fd, SOMAXCONN) == -1) {
            perror("listen");
            exit(EXIT_FAILURE);
        }

        // Add TCP socket to epoll
        add_fd_to_epoll(tcp_fd);

        // Create UDP socket
        udp_fd = socket(AF_INET, SOCK_DGRAM, 0);
        if (udp_fd == -1) {
            perror("socket");
            exit(EXIT_FAILURE);
        }

        // Bind UDP socket
        sockaddr_in udp_addr{};
        udp_addr.sin_family = AF_INET;
        udp_addr.sin_addr.s_addr = INADDR_ANY;
        udp_addr.sin_port = htons(PORT);
        if (bind(udp_fd, (struct sockaddr*)&udp_addr, sizeof(udp_addr)) == -1) {
            perror("bind");
            exit(EXIT_FAILURE);
        }

        // Add UDP socket to epoll
        add_fd_to_epoll(udp_fd);

        // Add signal pipe to epoll
        add_fd_to_epoll(signal_pipe[0]);
    }

    ~ServerEngine() {
        running = false;
        close(tcp_fd);
        close(udp_fd);
        close(signal_pipe[0]);
        close(signal_pipe[1]);
        close(epoll_fd);
    }

    void run() {
        while (running) {
            struct epoll_event events[MAX_EVENTS];
            int num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);

            for (int i = 0; i < num_events; ++i) {
                if (events[i].data.fd == tcp_fd) {
                    handle_new_tcp_connection();
                } else if (events[i].data.fd == udp_fd) {
                    handle_udp_message();
                } else if (events[i].data.fd == signal_pipe[0]) {
                    handle_signal_event();
                } else {
                    handle_client_message(events[i].data.fd);
                }
            }
        }
    }

private:
    int epoll_fd;
    int tcp_fd;
    int udp_fd;
    std::vector<ReadBuffer> read_buffers;
    std::vector<WriteBuffer> write_buffers;

    void add_fd_to_epoll(int fd) {
        epoll_event event{};
        event.events = EPOLLIN;
        event.data.fd = fd;
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1) {
            perror("epoll_ctl");
            exit(EXIT_FAILURE);
        }
    }

    void handle_new_tcp_connection() {
        sockaddr_in client_addr{};
        socklen_t addr_len = sizeof(client_addr);
        int client_fd = accept(tcp_fd, (struct sockaddr*)&client_addr, &addr_len);
        if (client_fd >= 0) {
            std::cout << "New TCP connection from " << inet_ntoa(client_addr.sin_addr) << std::endl;
            add_fd_to_epoll(client_fd);
            read_buffers.emplace_back();  // Create a ReadBuffer for the new client
            write_buffers.emplace_back(); // Create a WriteBuffer for the new client
        } else {
            perror("accept");
        }
    }

    void handle_udp_message() {
        char buffer[BUFFER_SIZE];
        sockaddr_in client_addr{};
        socklen_t addr_len = sizeof(client_addr);
        int len = recvfrom(udp_fd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &addr_len);
        if (len >= 0) {
            buffer[len] = '\0';
            std::cout << "Received UDP message: " << buffer << " from " << inet_ntoa(client_addr.sin_addr) << std::endl;

            // Echo the message back to the sender
            sendto(udp_fd, buffer, len, 0, (struct sockaddr*)&client_addr, addr_len);
        } else {
            perror("recvfrom");
        }
    }

    void handle_signal_event() {
        int signum;
        read(signal_pipe[0], &signum, sizeof(signum)); // Read signal from the pipe
        std::cout << "Signal received: " << signum << ". Sending message to all clients." << std::endl;
        
        // Here you could implement logic to send messages to all connected clients
        for (size_t i = 0; i < write_buffers.size(); ++i) {
            write_buffers[i].write("Hello from server!");
            write_buffers[i].flush(i);  // Here, you would need to provide the correct socket descriptor
        }
    }

    void handle_client_message(int client_fd) {
        ssize_t len = read_buffers[get_client_index(client_fd)].read(client_fd);
        if (len > 0) {
            std::string data = read_buffers[get_client_index(client_fd)].get_data();
            std::cout << "Received TCP message: " << data << std::endl;
            write_buffers[get_client_index(client_fd)].write(data);
            write_buffers[get_client_index(client_fd)].flush(client_fd);
            read_buffers[get_client_index(client_fd)].consume(data.size());
        } else if (len == 0) {
            std::cout << "Client disconnected." << std::endl;
            close(client_fd); // Close the connection
            read_buffers.erase(read_buffers.begin() + get_client_index(client_fd));
            write_buffers.erase(write_buffers.begin() + get_client_index(client_fd));
        } else {
            perror("read");
        }
    }

    int get_client_index(int client_fd) {
        // Find the index of the client based on the file descriptor
        for (size_t i = 0; i < read_buffers.size(); ++i) {
            if (read_buffers[i].current() == client_fd) {
                return i;
            }
        }
        return -1; // Not found
    }
};

// Function to simulate sending messages to clients
void message_sender() {
    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(5)); // Wait for some time before sending the message
        kill(getpid(), SIGUSR1); // Send signal to server engine to trigger sending a message
    }
}

int main() {
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

    ServerEngine server_engine;

    std::thread server_thread(&ServerEngine::run, &server_engine);
    std::thread sender_thread(message_sender);

    server_thread.join();
    sender_thread.join();

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}
