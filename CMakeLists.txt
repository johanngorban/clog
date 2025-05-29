cmake_minimum_required(VERSION 3.16)
project(clogs)

set(CMAKE_C_STANDARD, 11)

file(GLOB_RECURSE SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/source/*.c
)

add_library(clogs STATIC
    ${SOURCES}
)

target_include_directories(clogs
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)