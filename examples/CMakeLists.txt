cmake_minimum_required(VERSION 3.16)
project(example_clog)

set(BUILD_SHARED_LIB OFF)

find_package(loggy REQUIRED)

add_executable(example_clog
    ${CMAKE_CURRENT_SOURCE_DIR}/simple.c
)

target_link_libraries(example_clog
    PUBLIC
        loggy::loggy
)