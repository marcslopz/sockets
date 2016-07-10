#pragma once

#include <future>
#include <iostream>
#include <thread>

#include "../model/connection_data.h"
#include "../model/socket.h"
#include "../model/socket_data.h"
#include "../model/thread_safe_map.h"

namespace mjl {
namespace sockets {
namespace server {

    class minimum_thread_server {
    public:
        void start();
        void stop();
    private:
        void _accepter_method();
        void _sockets_reader_method();
        void _data_processor_method();
        void _sockets_writer_method();

        std::thread _accepter_thread;
        std::thread _sockets_reader_thread;
        std::thread _data_processor_thread;
        std::thread _sockets_writer_thread;

        std::promise<mjl::sockets::model::socket_data> _incoming_data_promise;
        std::promise<mjl::sockets::model::socket_data> _outgoing_data_promise;

        /// mjl::sockets::model::thread_safe_map<int,mjl::sockets::model::connection_data> _connections; FIXME
        mjl::sockets::model::thread_safe_map _connections;
        mjl::sockets::model::socket _socket;
    };

} /// namespace server
} /// namespace sockets
} /// namespace mjl
