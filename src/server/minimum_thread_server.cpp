#include "minimum_thread_server.h"

namespace mjlss = mjl::sockets::server;
namespace mjls = mjl::sockets;

void
mjlss::minimum_thread_server::start(){
    _socket.bind("6666");
    _accepter_thread = std::thread(
        &mjlss::minimum_thread_server::_accepter_method,
        this);

    std::clog << __FILE__ << "::" << __func__
                  << "::" << __LINE__ << " --> "
                  << "joining accepter_thread..."
                  << std::endl;

    _accepter_thread.join();
    /** FIXME
    _sockets_reader_thread = std::thread(
        &mjlss::minimum_thread_server::_sockets_reader_method,
        this);
    _data_processor_thread = std::thread(
        &mjlss::minimum_thread_server::_data_processor_method,
        this);
    _sockets_writer_thread = std::thread(
        &mjlss::minimum_thread_server::_sockets_writer_method,
        this);
    */
}

void
mjlss::minimum_thread_server::_accepter_method(){
    while(true){
        mjls::model::socket accepted_socket;

        std::clog << __FILE__ << "::" << __func__
                  << "::" << __LINE__ << " --> "
                  << "accepting next incoming connection..."
                  << std::endl;

        const int accept_errno_result = _socket.accept(accepted_socket);
        if (accept_errno_result != 0) {
            std::cerr << __FILE__ << "::" << __func__
                      << "::" << __LINE__ << " --> "
                      << "accept error '" << accept_errno_result << "'"
                      << std::endl;
            continue;
        }

        std::clog << __FILE__ << "::" << __func__
                  << "::" << __LINE__ << " --> "
                  << "accepted connection '" << "fixme:accepted_socket" << "'" /// FIXME!
                  << std::endl;

        const int accepted_socket_fd = accepted_socket.get_socket_fd();
        mjls::model::connection_data accepted_connection_data(std::move(accepted_socket));
        _connections.emplace(
            std::make_pair(
                accepted_socket_fd,
                accepted_connection_data));
    }
}


