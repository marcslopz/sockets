#pragma once

#include <string>
#include "socket.h"

namespace mjl {
namespace sockets {
namespace model {
    class socket_data {
    public:
    	socket_data(const socket&, const std::string&);

    	/// getters
    	const socket& get_socket() const;
    	std::string to_string() const;
    private:
        socket _socket;
        std::string _data;
    };
} /// model
} /// sockets
} /// mjl
