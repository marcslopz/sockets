#include "connection_data.h"

namespace mjlsm = mjl::sockets::model;

mjlsm::connection_data::connection_data(mjlsm::socket&& socket)
: _socket(socket)
{}
