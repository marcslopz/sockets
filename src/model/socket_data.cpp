#include "socket_data.h"

namespace mjlsm = mjl::sockets::model;

mjlsm::socket_data::socket_data(const socket& skt, const std::string& str)
: _socket(skt),
  _data(str)
{}
