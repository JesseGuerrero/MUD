cmake_minimum_required(VERSION 3.16.3)
project(echoserver)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")

add_subdirectory("client")
add_subdirectory("server")
