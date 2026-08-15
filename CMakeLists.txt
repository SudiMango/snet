cmake_minimum_required(VERSION 3.20)

project(snet LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SNET_SOURCES CONFIGURE_DEPENDS
  src/*.cpp
)

add_library(snet ${SNET_SOURCES})

target_include_directories(snet PUBLIC include)