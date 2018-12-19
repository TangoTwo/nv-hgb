cmake_minimum_required(VERSION 3.12)
project(ue08)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE CHESS_SRC "chess/*")
add_executable(chess ${CHESS_SRC})
