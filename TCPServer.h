//
// Created by Jaskaran Singh Puri on 05.02.26.
//

#ifndef HTTPSERVER_TCPSERVER_H
#define HTTPSERVER_TCPSERVER_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <unordered_map>
#include <functional>
#include <iostream>
#include "ThreadPool.h"

struct HTTPRequest {
    std::string method;
    std::string path;
    std::string version;
};

class TCPServer {
    private:
        std::string ip;
        int port;
        int m_socket;
        struct sockaddr_in server;
        unsigned int m_socketAddress_length;
        ThreadPool threadPool;

        void exit_with_error(const std::string& error_message);
        HTTPRequest parse_request(std::string request);
        std::string build_response(std::string content, int status_code);
        void handle_client(int client_socket);
        std::unordered_map<std::string, std::function<std::string()>> routes;
    public:
        TCPServer(std::string ip, int port) : ip(ip), port(port), threadPool(4) {;
            m_socket = socket(AF_INET, SOCK_STREAM, 0);

            int opt = 1;
            if (setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
                exit_with_error("Failed to set socket options");
            }

            m_socketAddress_length = sizeof(server);
            server.sin_family = AF_INET;
            server.sin_port = htons(port);

            if (inet_pton(AF_INET, ip.c_str(), &server.sin_addr) <= 0) {
                exit_with_error("Invalid address/ Address not supported");
            }
        }
        ~TCPServer();
        int startserver();
        void add_route(std::string path, std::function<std::string()> handler);
};

#endif //HTTPSERVER_TCPSERVER_H