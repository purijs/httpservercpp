//
// Created by Jaskaran Singh Puri on 05.02.26.
//

#include "TCPServer.h"
#include <sstream>
#include <functional>
#include "HttpResponse.h"

TCPServer::~TCPServer() {
    close(m_socket);
    std::cout << "Closing TCP Server" << std::endl;
}

void TCPServer::exit_with_error(const std::string& error_message) {
    perror(error_message.c_str());
    exit(EXIT_FAILURE);
}

void TCPServer::add_route(std::string path, std::function<std::string()> handler) {
    routes[path] = handler;
}

void TCPServer::handle_client(int client_socket) {
    char buffer[8192] = {0};
    ssize_t bytes_read = read(client_socket, buffer, 8192);

    if (bytes_read > 0) {
        std::string request(buffer);
        HTTPRequest request_data = parse_request(request);

        std::string content;
        int status_code = 200;

        if (routes.find(request_data.path) != routes.end()) {
            std::function<std::string()> handler = routes[request_data.path];
            content = handler();
            status_code = 200;
        } else {
            HttpResponse<std::string> res("<h1>404 Not Found</h1>", "text/html", 404);
            status_code = 404;
            content = res.serialize();
        }

        std::cout << "Received Request: " << request_data.method << " " << request_data.path << " " << request_data.version << std::endl;

        ssize_t bytes_written = write(client_socket, content.c_str(), content.size());

        if (bytes_written == content.size()) {
            std::cout << "Response Sent" << std::endl;
        }

        std::cout << "Thread: " << std::this_thread::get_id() << " finished request for: " << request_data.path << std::endl;

    }
    close(client_socket);
}

HTTPRequest TCPServer::parse_request(std::string request) {
    HTTPRequest request_data;
    std::istringstream request_stream(request);

    request_stream >> request_data.method >> request_data.path >> request_data.version;
    return request_data;
}

int TCPServer::startserver() {
    if (bind(m_socket, (struct sockaddr *)&server, sizeof(server)) < 0) {
        exit_with_error("Bind failed");
    }

    if (listen(m_socket, 10) < 0) {
        exit_with_error("Listen failed");
    }

    std::cout << "Listening on ADDRESS: " << ip << " PORT: " << port << std::endl;

    while (true) {
        int new_socket = accept(m_socket, (struct sockaddr *)&server, &m_socketAddress_length);
        if (new_socket < 0) {
            exit_with_error("Accept failed");
        }

        std::cout << "Connection established" << std::endl;
        threadPool.enqueue(std::bind(&TCPServer::handle_client, this, new_socket));

    }
    close(m_socket);
    return 0;
}
