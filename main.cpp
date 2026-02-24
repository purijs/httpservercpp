#include <iostream>
#include "TCPServer.h"
#include "FileReader.h"
#include "HttpResponse.h"

int main() {
    TCPServer server("127.0.0.1", 8080);

    server.add_route("/", []() -> std::string {
        HttpResponse<std::string> res("<html><h1>Index Page</h1></html>", "text/html");
        return res.serialize();
    });

    server.add_route("/image", []() -> std::string {
        try {
            FileReader reader("/local/path/image.jpg");
            FileContent content = reader.read_all();
            HttpResponse<FileContent> res(std::move(content), "image/jpeg");
            return res.serialize();
        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
            return "<html><h1>File Not Found</h1></html>";
        }
    });

    server.add_route("/load", []() -> std::string {
        try {
            FileReader reader("index.html");
            FileContent content = reader.read_all();
            HttpResponse<FileContent> res(std::move(content), "text/html");
            return res.serialize();
        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
            return "<html><h1>File Not Found</h1></html>";
        }
    });

    server.add_route("/user", []() -> std::string {
        std::string jsondata = "{\"user\": \"abc\", \"role\": \"Admin\"}";
        HttpResponse<std::string> res(jsondata, "application/json");
        return res.serialize();
    });

    server.startserver();
    return 0;
}