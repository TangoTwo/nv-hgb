cmake_minimum_required(VERSION 3.12)
project(ue07)

set(CMAKE_CXX_STANDARD 17)

add_library(compile_flags INTERFACE)
target_compile_options(compile_flags INTERFACE -Wall -Wconversion -Werror=vla -Wextra -Wpedantic)

file(GLOB_RECURSE WMB_SRC "src/wmb/*")
add_executable(wmb ${WMB_SRC})
target_link_libraries(wmb compile_flags)

file(GLOB_RECURSE GRAPH_SRC "src/graph/*")
add_executable(graph ${GRAPH_SRC})
target_link_libraries(graph compile_flags)