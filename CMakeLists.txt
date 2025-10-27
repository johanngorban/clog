cmake_minimum_required(VERSION 3.16)
project(loggy VERSION 1.3.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

option(BUILD_SHARED_LIB "Build using shared lib" ON)

file(GLOB_RECURSE SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/loggy/*.c
)

if (WIN32 AND BUILD_SHARED_LIB)
    add_definitions(-DLOGGY_EXPORTS)
endif()

if (BUILD_SHARED_LIB)
    add_library(loggy SHARED ${SOURCES})
else()
    add_library(loggy STATIC ${SOURCES})
endif()

if (APPLE AND BUILD_SHARED_LIB)
    set_target_properties(loggy
        PROPERTIES
            MACOSX_RPATH OFF
            INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}"
    )
endif()

target_include_directories(loggy
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/loggy>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(TARGETS loggy
    EXPORT loggyTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/loggy/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h"
)

install(EXPORT loggyTargets
    FILE loggyTargets.cmake
    NAMESPACE loggy::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/loggy
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/loggyConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/loggyConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/loggyConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/loggy
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/loggyConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/loggyConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/loggy
)
