#pragma once

#include "socket.h"

namespace mjl {
namespace sockets {
namespace model {

    class connection_data {
    public:
        connection_data(socket&&);
    private:
        socket _socket;
    };
} /// namespace model
} /// namespace sockets
} /// namespace mjl
