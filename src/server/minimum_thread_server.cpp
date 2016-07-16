#include "minimum_thread_server.h"

namespace mjlss = mjl::sockets::server;
namespace mjls = mjl::sockets;
namespace mjlst = mjl::sockets::tools;
namespace mjlsm = mjl::sockets::model;

mjlss::minimum_thread_server::minimum_thread_server(
    const mjl::sockets::tools::logger& logger) :
    _socket(logger), _logger(logger) {
}

void mjlss::minimum_thread_server::start() {
  _socket.bind("6666");
  _accepter_thread = std::thread(
      &mjlss::minimum_thread_server::_accepter_method, this);

  _sockets_reader_thread = std::thread(
      &mjlss::minimum_thread_server::_sockets_reader_method, this);

  std::clog << __FILE__ << "::" << __func__ << "::" << __LINE__ << " --> "
      << "joining accepter_thread..." << std::endl;
  _accepter_thread.join();

  std::clog << __FILE__ << "::" << __func__ << "::" << __LINE__ << " --> "
      << "joining reader thread..." << std::endl;
  _sockets_reader_thread.join();
  /** FIXME

   _data_processor_thread = std::thread(
   &mjlss::minimum_thread_server::_data_processor_method,
   this);
   _sockets_writer_thread = std::thread(
   &mjlss::minimum_thread_server::_sockets_writer_method,
   this);
   */
}

void mjlss::minimum_thread_server::_accepter_method() {
  _logger.to_ostream(
      std::clog,
      __FILE__,
      __LINE__,
      "ACCEPTER: starting thread",
      mjlst::log_level::debug);

  while (true) {
    mjls::model::socket accepted_socket(_logger);

    _logger.to_ostream(
        std::clog,
        __FILE__,
        __LINE__,
        "ACCEPTER: accepting next incoming connection...",
        mjlst::log_level::debug);

    const int accept_errno_result = _socket.accept(accepted_socket);
    if (accept_errno_result != 0) {
      _logger.to_ostream(
          std::cerr,
          __FILE__,
          __LINE__,
          "ACCEPTER: accept error errno='" +
            std::to_string(accept_errno_result) + "'",
          mjlst::log_level::error);
      continue;
    }

    _logger.to_ostream(
        std::clog,
        __FILE__,
        __LINE__,
        "ACCEPTER: accepted connection '" + accepted_socket.to_string() + "'",
        mjlst::log_level::debug);

    _connections.insert(
        std::make_pair(accepted_socket.get_socket_fd(), accepted_socket));

    _logger.to_ostream(
        std::clog,
        __FILE__,
        __LINE__,
        "ACCEPTER: notify reader thread",
        mjlst::log_level::debug);
//    _incoming_connection_promise.set_value();
  }
}

void mjlss::minimum_thread_server::_sockets_reader_method() {
  _logger.to_ostream(
      std::clog,
      __FILE__,
      __LINE__,
      "READER: starting reader thread",
      mjlst::log_level::debug);
  while (true) {
//    if (_connections.empty()) {
//      auto incoming_connection_future =
//          _incoming_connection_promise.get_future();
//      /// wait for new connection
//      _logger.to_ostream(
//          std::clog,
//          __FILE__,
//          __LINE__,
//          "READER: empty accepted connections, waiting for incoming connection",
//          mjlst::log_level::debug);
//      incoming_connection_future.get();
//    }

    _logger.to_ostream(
        std::clog,
        __FILE__,
        __LINE__,
        "READER: waiting for incoming data in any of the accepted connections",
        mjlst::log_level::debug);
    fd_set read_socket_fds;
    const int max_socket_fd = _connections.get_select_info(read_socket_fds);
    //int select(int nfds, fd_set *readfds, fd_set *writefds,
    //        fd_set *exceptfds, struct timeval *timeout);
    const int select_result =
        select(max_socket_fd + 1, &read_socket_fds, nullptr, nullptr, nullptr);
    if (select_result == -1) {
      _logger.to_ostream(std::cerr,
      __FILE__,
      __LINE__,
      "READER: select returned -1!",
      mjlst::log_level::alert);
      exit(1);
    }
    _logger.to_ostream(
        std::clog,
        __FILE__,
        __LINE__,
        "READER: data available, selecting accepted connection",
        mjlst::log_level::debug);
    const mjlsm::socket* selected_socket =
        _connections.get_first_selected_socket(read_socket_fds);
    if (!selected_socket) {
      _logger.to_ostream(std::clog,
      __FILE__,
      __LINE__,
      "null pointer returned in get_first_selected_socket",
      mjlst::log_level::alert);
      exit(1);
    }

    _logger.to_ostream(
        std::clog,
        __FILE__,
        __LINE__,
        "READER: data available, socket '" + selected_socket->to_string() + "'",
        mjlst::log_level::debug);
    const std::string socket_data = selected_socket->read_data();
    if (socket_data.empty()) {
      _logger.to_ostream(std::clog,
      __FILE__,
      __LINE__,
          "closing socket '" + selected_socket->to_string() + "'",
          mjlst::log_level::error);
      _connections.erase(selected_socket->get_socket_fd());
      continue;
    }

    _logger.to_ostream(
        std::clog,
        __FILE__,
        __LINE__,
        "READER: data received, notify to data processor thread",
        mjlst::log_level::debug);
    const mjlsm::socket_data my_connection_data(*selected_socket, socket_data);
    delete selected_socket;
//    FIXME: when future of incoming data is done
//    _incoming_data_promise.set_value(my_connection_data);
  }
}
