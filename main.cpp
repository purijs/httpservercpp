#include <iostream>
#include "TCPServer.h"

int main() {
    TCPServer server("127.0.0.1", 8080);
    server.startserver();
    return 0;
}