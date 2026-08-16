cmake_minimum_required(VERSION 3.20)

project(snet LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SNET_SOURCES CONFIGURE_DEPENDS
  src/*.cpp
)

add_library(snet ${SNET_SOURCES})

target_include_directories(snet PUBLIC include)

add_executable(udp_chat_server
  examples/udp_chat/server.cpp
)
target_link_libraries(udp_chat_server PRIVATE snet)

add_executable(udp_chat_client
  examples/udp_chat/client.cpp
)
target_link_libraries(udp_chat_client PRIVATE snet)
