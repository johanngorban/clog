cmake_minimum_required(VERSION 3.16)
project(liblogs)

set(CMAKE_C_STANDARD, 11)

file(GLOB_RECURSE SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/source/*.c
)

add_library(liblogs STATIC
    ${SOURCES}
)

target_include_directories(liblogs
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)