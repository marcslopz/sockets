#include <sys/select.h>

#include "server/minimum_thread_server.h"
#include "tools/logger.h"

namespace mjls = mjl::sockets;

int main(int argc, char** argv){
    mjls::tools::logger my_logger(mjls::tools::log_level::debug);
    mjls::server::minimum_thread_server my_server(my_logger);

    my_server.start();

    fd_set set;
    FD_SET(1, &set);
    return 0;

}
