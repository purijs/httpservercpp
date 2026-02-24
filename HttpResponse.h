//
// Created by Jaskaran Singh Puri on 24.02.26.
//

#ifndef HTTPSERVER_HTTPRESPONSE_H
#define HTTPSERVER_HTTPRESPONSE_H

#include <string>
#include <sstream>
#include "FileReader.h"

template<typename T>
class HttpResponse {
    private:
        T content;
        int status_code;
        std::string content_type;

    public:
        HttpResponse(T c, std::string type, int code = 200) {
            content = std::move(c);
            content_type = type;
            status_code = code;
        }

    std::string serialize();
};

template <>
inline std::string HttpResponse<std::string>::serialize() {
    std::ostringstream response;
    response << "HTTP/1.1 " << status_code << " OK\r\n";
    response << "Content-Type: " << content_type << "\r\n";
    response << "Content-Length: " << content.length() << "\r\n";
    response << "Connection: close\r\n\r\n";
    response << content;
    return response.str();
}

template <>
inline std::string HttpResponse<FileContent>::serialize() {
    std::ostringstream response;
    response << "HTTP/1.1 " << status_code << " OK\r\n";
    response << "Content-Type: " << content_type << "\r\n";
    response << "Content-Length: " << content.size << "\r\n";
    response << "Connection: close\r\n\r\n";

    std::string full_response = response.str();
    full_response.append(content.data.get(), content.size);
    return full_response;
}

#endif //HTTPSERVER_HTTPRESPONSE_H