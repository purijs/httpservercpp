//
// Created by Jaskaran Singh Puri on 23.02.26.
//

#ifndef HTTPSERVER_FILEREADER_H
#define HTTPSERVER_FILEREADER_H

#include <string>
#include <memory>
#include <stdexcept>
#include <cstdio>

struct FileContent {
    std::unique_ptr<char[]> data; //smart pointer to dynamic array
    size_t size;
};

class FileReader {
    private:
        FILE* file_ptr;

    public:
        FileReader(const std::string& filepath) {
            file_ptr = fopen(filepath.c_str(), "rb");
            if (!file_ptr) {
                throw std::runtime_error("Couldn't open file: " + filepath);
            }
        }

        ~FileReader() {
            if (file_ptr) {
                fclose(file_ptr);
            }
        }

        FileContent read_all() {
            fseek(file_ptr, 0, SEEK_END);
            size_t file_size = ftell(file_ptr);
            fseek(file_ptr, 0, SEEK_SET);

            if (file_size < 0) throw std::runtime_error("Couldn't determine file size");

            std::unique_ptr<char[]> buffer = std::make_unique<char[]>(file_size);
            fread(buffer.get(), 1, file_size, file_ptr);
            return {
                std::move(buffer), file_size
            };
        }


};

#endif //HTTPSERVER_FILEREADER_H