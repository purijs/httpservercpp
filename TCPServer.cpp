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

void TCPServer::add_route(std::string path, std::function<std::string()> handler) {
    routes[path] = handler;
}


HTTPRequest TCPServer::parse_request(std::string request) {
    HTTPRequest request_data;
    std::istringstream request_stream(request);

    request_stream >> request_data.method >> request_data.path >> request_data.version;
    return request_data;
}

std::string TCPServer::build_response(std::string content, int status_code) {
    std::string status_text;

    if (status_code == 200) { status_text = "OK"; }
    else if (status_code == 404) { status_text = "Not Found"; }
    else status_text = "Internal Server Error";

    std::ostringstream response_stream;
    response_stream << "HTTP/1.1 " << status_code << " : " << status_text << "\r\n";
    response_stream << "Content-Type: text/html\r\n";
    response_stream << "Content-Length: " << content.length() << "\r\n";
    response_stream << "Server: CPPServer/1.0\r\n";
    response_stream << "\r\n";

    response_stream << content;
    return response_stream.str();
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

        char buffer[1024] = {0};
        ssize_t bytes_read = read(new_socket, buffer, 1024);

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
                content = "<html><body><h1>404 Not Found</h1></body></html>";
                status_code = 404;
            }

            std::string response = build_response(content, status_code);
            std::cout << "Received Request: " << request_data.method << " " << request_data.path << " " << request_data.version << std::endl;

            ssize_t bytes_written = write(new_socket, response.c_str(), response.size());

            if (bytes_written == response.size()) {
                std::cout << "Response Sent" << std::endl;
            }

        }
        close(new_socket);
    }
    close(m_socket);
    return 0;
}
