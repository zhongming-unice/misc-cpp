cmake_minimum_required(VERSION 2.6)

project(echo CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(echoClient echoClient.cc)
add_executable(echoPollServer echoPollServer.cc)
add_executable(echoEpollServer echoEpollServer.cc)
