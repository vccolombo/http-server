cmake_minimum_required(VERSION 3.22)
project(http_server)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-O2 -Wall -Wextra -Wpedantic -Werror -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Wuninitialized -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default)
endif()

add_executable(http_server src/main.cpp src/server.cpp src/server.hpp src/application.hpp src/connection.cpp src/connection.hpp src/applicationfactory.hpp src/writer.hpp src/request.hpp src/response.cpp src/response.hpp src/router.cpp src/router.hpp src/httpserver.cpp src/httpserver.hpp src/httpparse.hpp src/logger.cpp src/logger.hpp src/using.hpp)
