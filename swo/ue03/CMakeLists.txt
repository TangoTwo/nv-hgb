cmake_minimum_required(VERSION 3.12)
project(ue03 C)

set(CMAKE_C_STANDARD 11)

add_executable(sudoku sudoku/main.c sudoku/SudokuField.c sudoku/Sudoku.c)
add_executable(pipe pipe/pipe.c)
target_compile_options(sudoku PUBLIC -Wall -Wconversion -Werror=vla -Wextra -Wpedantic)
target_compile_options(pipe PUBLIC -Wall -Wconversion -Werror=vla -Wextra -Wpedantic)