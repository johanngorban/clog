project(example_clog)

find_package(clog REQUIRED)

add_executable(example_clog
    ${CMAKE_CURRENT_SOURCE_DIR}/simple.c
)

target_link_libraries(example_clog
    PUBLIC
        clog
)