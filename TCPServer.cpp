//
// Created by Jaskaran Singh Puri on 05.02.26.
//

#include "TCPServer.h"
#include <sstream>

TCPServer::~TCPServer() {
    close(m_socket);
    std::cout << "Closing TCP Server" << std::endl;
}

void TCPServer::exit_with_error(const std::string& error_message) {
    perror(error_message.c_str());
    exit(EXIT_FAILURE);
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

    int new_socket = accept(m_socket, (struct sockaddr *)&server, &m_socketAddress_length);
    if (new_socket < 0) {
        exit_with_error("Accept failed");
    }

    std::cout << "Connection established" << std::endl;

    char buffer[1024] = {0};
    ssize_t bytes_read = read(new_socket, buffer, 1024);

    if (bytes_read > 0) {
        std::string request(buffer);
        HTTPRequest request_data = parse_request(request);

        std::cout << "Received Request: " << request_data.method << " " << request_data.path << " " << request_data.version << std::endl;
    }

    std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 20\r\n\r\n<b>Hello, World!</b>";
    ssize_t bytes_written = write(new_socket, response.c_str(), response.size());

    if (bytes_written == response.size()) {
        std::cout << "Response Sent" << std::endl;
    }

    close(m_socket);

    return 0;
}
