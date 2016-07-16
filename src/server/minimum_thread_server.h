#pragma once

#include <sys/select.h>

#include <future>
#include <iostream>
#include <thread>

#include "../model/socket.h"
#include "../model/socket_data.h"
#include "../model/thread_safe_map.h"
#include "../model/thread_safe_vector.h"
#include "../tools/logger.h"

using mjl::sockets::model::thread_safe_map;
using mjl::sockets::model::thread_safe_vector;

namespace mjl {
namespace sockets {
namespace server {

    class minimum_thread_server {
    public:
        minimum_thread_server(const mjl::sockets::tools::logger&);
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

//        std::condition_variable _incoming_connection_condition;
//        std::mutex _incoming_connection_mutex;
//        std::promise<void> _incoming_connection_promise;

//        std::condition_variable _incoming_data_condition;
//        std::mutex _incoming_data_mutex;
        std::promise<mjl::sockets::model::socket_data> _incoming_data_promise;

//        std::condition_variable _outgoing_data_condition;
//        std::mutex _outgoing_data_mutex;
        std::promise<mjl::sockets::model::socket_data> _outgoing_data_promise;

        thread_safe_map<mjl::sockets::model::socket> _connections;
        mjl::sockets::model::socket _socket;

        mjl::sockets::tools::logger _logger;
    };

} /// namespace server
} /// namespace sockets
} /// namespace mjl
