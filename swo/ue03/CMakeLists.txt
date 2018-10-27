cmake_minimum_required(VERSION 3.12)
project(ue03 C)

set(CMAKE_C_STANDARD 11)

add_library(compile_flags INTERFACE)
target_compile_options(compile_flags INTERFACE -Wall -Wconversion -Werror=vla -Wextra -Wpedantic)

# sudoku
file(GLOB_RECURSE SUDOKU_SRC "src/sudoku/*")
add_executable(sudoku ${SUDOKU_SRC})
target_link_libraries(sudoku compile_flags)

# pipe
file(GLOB_RECURSE PIPE_SRC "src/pipe/*")
add_executable(pipe ${PIPE_SRC})
target_link_libraries(pipe compile_flags)
