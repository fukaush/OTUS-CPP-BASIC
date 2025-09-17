cmake_minimum_required(VERSION 3.8)

project(statistics)

add_executable(statistics statistics.cpp)

set_target_properties(statistics PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)