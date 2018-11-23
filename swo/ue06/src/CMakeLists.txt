cmake_minimum_required(VERSION 3.12)
project(ue06)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(compile_flags INTERFACE)
if("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(compile_flags INTERFACE -Wall -Wextra -Wpedantic -Wconversion -Werror=vla)
endif()

find_package(glfw3  REQUIRED)
find_package(OpenGL REQUIRED)

file(GLOB_RECURSE TETRIS "tetris/*")
add_executable(tetris ${TETRIS})
target_link_libraries(tetris compile_flags glfw OpenGL::GL OpenGL::GLU)
