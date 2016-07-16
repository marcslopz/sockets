#include "socket.h"

namespace mjlsm = mjl::sockets::model;
namespace mjlst = mjl::sockets::tools;

mjlsm::socket::socket(const mjlst::logger& logger)
    : _logger(logger),
      _socket_fd(-1)
{}

int
mjlsm::socket::bind(const std::string& port) {
    if (_socket_fd != -1) {
        _logger.to_ostream(
            std::cerr,
            __FILE__,
            __LINE__,
            "socket already binded!!",
            mjlst::log_level::error);
        return -1;
    }

    addrinfo hints;
    hints.ai_family = AF_INET; /// ipv4
    hints.ai_socktype = SOCK_STREAM; /// tcp
    hints.ai_protocol = 0; /// any protocol
    hints.ai_flags = AI_PASSIVE; /// allow binding in any local machine address

    addrinfo* addresses_returned = nullptr;
    const int get_addr_result =
        getaddrinfo(nullptr, port.c_str(), &hints, &addresses_returned);

    if (get_addr_result) {
        _logger.to_ostream(
            std::cerr,
            __FILE__,
            __LINE__,
            "getaddrinfo error '" + std::to_string(get_addr_result) + "'",
            mjlst::log_level::error);
        return get_addr_result;
    }

    std::clog << __FILE__ << "::" << __func__
              << "::" << __LINE__ << " --> "
              << "getaddrinfo ok: addresses [";
    for (const addrinfo* address = addresses_returned; address; address = address->ai_next) {
        const in_addr* const inet_address = (const in_addr* const)(address->ai_addr);
        char* inet_address_c_str = inet_ntoa(*inet_address);
        std::clog << inet_address_c_str << ", ";
    }
    std::clog << "]" << std::endl;

    for (const addrinfo* address = addresses_returned; address; address = address->ai_next) {
        std::clog << __FILE__ << "::" << __func__
                  << "::" << __LINE__ << " --> "
                  << "trying to bind address '";
        const in_addr* const inet_address = (const in_addr* const)(address->ai_addr);
        char* inet_address_c_str = inet_ntoa(*inet_address);
        std::clog << inet_address_c_str << "'....";

        const int socket_result =
            ::socket(
                address->ai_family,
                address->ai_socktype,
                address->ai_protocol);
        if (socket_result == -1) {
            std::clog << "KO socket_result='" << socket_result << "'" << std::endl;
            continue;
        }
        const int bind_result =
            ::bind(socket_result, address->ai_addr, address->ai_addrlen);
        if (bind_result != 0) {
            std::clog << "KO bind_result='" << bind_result << "'" << std::endl;
            continue;
        }

        const int max_listen_queue = 1024;
        errno = 0;
        const int listen_result =
            ::listen(socket_result, max_listen_queue);

        if (listen_result != 0) {
            const int errno_cpy = errno;
            std::clog << "KO listen_errno='" << errno_cpy << "'" << std::endl;
            continue;
        }

        std::clog << "OK!" << std::endl;
        _socket_fd = socket_result;
        _local_port = port;
        _local_ip = inet_address_c_str;
        break;
    }

    freeaddrinfo(addresses_returned);

    return (_socket_fd == -1)? _socket_fd: 0;
}

int
mjlsm::socket::accept(mjlsm::socket& socket) {
    if (_socket_fd == -1) {
        _logger.to_ostream(
            std::cerr,
            __FILE__,
            __LINE__,
            "socket is not binded!!",
            mjlst::log_level::error);
        return -1;
    }
    errno = 0;
    sockaddr peer_address;
    socklen_t peer_address_length;
    const int accept_result =
        ::accept(_socket_fd, &peer_address, &peer_address_length);
    if (accept_result == -1) {
        const int errno_cpy = errno;
        _logger.to_ostream(
            std::cerr,
            __FILE__,
            __LINE__,
            "error in accept, errno='" + std::to_string(errno_cpy) + "'",
            mjlst::log_level::error);
        return errno_cpy;
    }
    in_addr& peer_inet_address = (in_addr&)peer_address;
    const std::string inet_address_str = inet_ntoa(peer_inet_address);
    socket.set_peer_ip(inet_address_str);
    socket.set_socket_fd(accept_result);
    return 0;
}

void
mjlsm::socket::set_peer_ip(const std::string& s) {_peer_ip = s;}

void
mjlsm::socket::set_socket_fd(int i) {_socket_fd = i;}

int
mjlsm::socket::get_socket_fd() const {return _socket_fd;}

std::string
mjlsm::socket::read_data() const {
  std::string read_data;
  if (_socket_fd == -1) {
    _logger.to_ostream(
        std::cerr,
        __FILE__,
        __LINE__,
        "cannot read data from socket, fd is -1",
        mjlst::log_level::error);
    return read_data;
  }
  _logger.to_ostream(
      std::clog,
      __FILE__,
      __LINE__,
      "socket='" + std::to_string(_socket_fd) + "', starting to read...",
      mjlst::log_level::debug);
  const size_t max_read_size = 1024;
  read_data.resize(max_read_size);
  errno = 0;
  ssize_t recv_result = recv(_socket_fd, &read_data[0], max_read_size, 0);
  if (recv_result == -1) {
    const int errno_cpy = errno;
    _logger.to_ostream(
        std::cerr,
        __FILE__,
        __LINE__,
        "socket='" + std::to_string(_socket_fd) + "', error in read, errno='" +
          std::to_string(errno_cpy) + "', read_data='" + read_data + "'",
        mjlst::log_level::error);
    return read_data;
  }
  read_data.resize(static_cast<size_t>(recv_result));
  _logger.to_ostream(
      std::clog,
      __FILE__,
      __LINE__,
      "socket='" + std::to_string(_socket_fd) + "', read OK msg='" +
        read_data + "' (" + std::to_string(read_data.size()) + " bytes)",
      mjlst::log_level::debug);
  return read_data;
}

std::string
mjlsm::socket::to_string() const {
  std::ostringstream oss;
  oss << "[socket][_socket_fd=" << _socket_fd << "][_local_ip=" << _local_ip
      << "][_local_port=" << _local_port << "][_peer_ip=" << _peer_ip
      << "][_peer_port=" << _peer_port << "]";
  return oss.str();
}

void
mjlsm::socket::close() {
  if (_socket_fd == -1) {
    _logger.to_ostream(
        std::cerr,
        __FILE__,
        __LINE__,
        "trying to close a socket which has _socket_fd=-1",
        mjlst::log_level::error);
    return;
  }
  ::close(_socket_fd);
  _logger.to_ostream(
      std::clog,
      __FILE__,
      __LINE__,
      "'" + to_string() + "' closed OK",
      mjlst::log_level::debug);
  _socket_fd = -1;
}


