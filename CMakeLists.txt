cmake_minimum_required(VERSION 3.16)
project(clog VERSION 1.3.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

file(GLOB_RECURSE SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/clog/*.c
)

option(BUILD_SHARED_LIB, "Build using shared lib", ON)

target_include_directories(clog
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(TARGETS clog
    EXPORT clogTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/clog/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/clog
    FILES_MATCHING PATTERN "*.h"
)

install(EXPORT clogTargets
    FILE clogTargets.cmake
    NAMESPACE clog::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/clog
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/clogConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/clogConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/clogConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/clog
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/clogConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/clogConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/clog
)
