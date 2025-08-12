cmake_minimum_required(VERSION 3.16)
project(clog)

set(CMAKE_C_STANDARD, 11)

file(GLOB_RECURSE SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/clog/*.c
)

add_library(clog STATIC
    ${SOURCES}
)

target_include_directories(clog
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/clog
)