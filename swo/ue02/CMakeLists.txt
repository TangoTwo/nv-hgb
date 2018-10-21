cmake_minimum_required(VERSION 3.12)
project(ue02 C)

set(CMAKE_C_STANDARD 11)

add_executable(hamming_sequence src/hamming/hamming.c)
add_executable(gross src/gross/gross.c)
add_executable(sort src/sort/sort.c)
target_compile_options(hamming_sequence PUBLIC -Wall -Wconversion -Werror=vla -Wextra -Wpedantic)
target_compile_options(gross PUBLIC -Wall -Wconversion -Werror=vla -Wextra -Wpedantic)
target_compile_options(sort PUBLIC -Wall -Wconversion -Werror=vla -Wextra -Wpedantic)