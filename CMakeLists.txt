cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(http_server CXX)

set(BOOST_ROOT "C:/boost")

find_package(Boost 1.90 REQUIRED COMPONENTS filesystem)

add_executable(http_server
    http_server.cpp
    http_route.hpp
    http_router.hpp
)

target_link_libraries(http_server PRIVATE
    Boost::filesystem
)

if (MSVC)
    target_compile_options(http_server PUBLIC /W4)
else ()
    target_compile_options(http_server PUBLIC -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion)
endif ()