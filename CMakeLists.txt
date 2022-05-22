cmake_minimum_required(VERSION 3.22)
project(http_server)

set(CMAKE_CXX_STANDARD 23)

add_executable(http_server src/main.cpp src/server.cpp src/server.hpp src/application.hpp src/connection.cpp src/connection.hpp src/applicationfactory.hpp src/writer.hpp src/request.hpp src/response.cpp src/response.hpp src/router.cpp src/router.hpp src/httpserver.cpp src/httpserver.hpp)
