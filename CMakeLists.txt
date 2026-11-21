
cmake_minimum_required(VERSION 3.21)

project(FoxyFua VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(
    $ENV{GEODE_SDK}
    ${CMAKE_CURRENT_BINARY_DIR}/geode
)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    src/*.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

setup_geode_mod(${PROJECT_NAME})
