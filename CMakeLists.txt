cmake_minimum_required(VERSION 3.8)

project(containers)

add_executable(containers vector.hpp containers.cpp
        list.hpp)

set_target_properties(containers PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)