cmake_minimum_required(VERSION 3.12)
project(ue09)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE DEQUE_SRC "deque/*")
add_executable(deque ${DEQUE_SRC})
