cmake_minimum_required(VERSION 3.25)
project(ChatApp VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 17)

add_library(LOCAL ${CMAKE_BINARY_DIR}/../local.hpp)
set_target_properties(LOCAL PROPERTIES LINKER_LANGUAGE CXX)

add_executable(client client.cpp)
add_executable(server server.cpp)

target_link_libraries(client LOCAL)
target_link_libraries(server LOCAL)
