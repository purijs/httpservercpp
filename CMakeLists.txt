cmake_minimum_required(VERSION 4.0)
project(httpserver)

set(CMAKE_CXX_STANDARD 20)

add_executable(httpserver main.cpp
        TCPServer.h
        TCPServer.cpp
        ThreadPool.h
        ThreadPool.cpp
        FileReader.h)
