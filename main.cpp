#include <iostream>
#include "TCPServer.h"

int main() {
    TCPServer server("127.0.0.1", 8080);

    server.add_route("/", []() -> std::string {
       return "<html><h1>Index Page</h1></html>";
    });

    server.add_route("/status", []() -> std::string {
        return "<html><h1>Status Page</h1></html>";
    });

    server.add_route("/user", []() -> std::string {
        return "{\"user\": \"abc\", \"role\": \"Admin\"}";
    });

    server.startserver();
    return 0;
}