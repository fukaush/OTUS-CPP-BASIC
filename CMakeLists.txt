cmake_minimum_required(VERSION 3.5)

project(containers)

# Download and build googletest
# @see https://github.com/google/googletest/blob/master/googletest/README.md
# Incorporating Into An Existing CMake Project

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()
# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)
# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif()

add_executable(
        container
        containers.cpp
        list.hpp
        vector.hpp
)

add_executable(
        container_tests
        list_tests.cpp
        vector_tests.cpp
)

# Add src to the include directories for the test target
target_include_directories(
        container_tests PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

# Link test target against gtest libraries
target_link_libraries(
        container_tests PRIVATE
        gtest gtest_main
)

set_target_properties(container PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
set_target_properties(container_tests PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

if (MSVC)
    target_compile_options(
            container PRIVATE
            /W4
    )
    target_compile_options(
            container_tests PRIVATE
            /W4
    )
else ()
    target_compile_options(
            container PRIVATE
            -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(
            container_tests PRIVATE
            -Wall -Wextra -pedantic -Werror
    )
endif()