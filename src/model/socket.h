#pragma once

#include <iostream>
#include <sstream>
#include <string>

#include <arpa/inet.h>
#include <cerrno>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../tools/logger.h"

namespace mjl {
namespace sockets {
namespace model {

    class socket {
    public:
        socket(const mjl::sockets::tools::logger&);

        /// server methods
        int accept(socket&);
        int bind(const std::string&);

        /// client methods
        int connect_to(std::string&, std::string&);

        void close();

        /// I/O operations
        std::string read_data() const;

        /// getters
        int get_socket_fd() const;
        std::string to_string() const;

        /// setters
        void set_socket_fd(int);
        void set_peer_ip(const std::string&);
    private:
        const mjl::sockets::tools::logger& _logger;
        int _socket_fd;

        std::string _local_ip;
        std::string _local_port;

        std::string _peer_ip;
        std::string _peer_port;
    };

} /// namespace model
} /// namespace sockets
} /// namespace mjl


