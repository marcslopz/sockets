#include "server/minimum_thread_server.h"

namespace mjls = mjl::sockets;

int main(int argc, char** argv){
    mjls::server::minimum_thread_server my_server;

    my_server.start();

    return 0;

}
