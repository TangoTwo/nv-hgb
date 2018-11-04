cmake_minimum_required(VERSION 3.12)
project(ue04 C)

set(CMAKE_C_STANDARD 11)

add_library(compile_flags INTERFACE)
target_compile_options(compile_flags INTERFACE -Wall -Wconversion -Werror=vla -Wextra -Wpedantic)

include_directories(./src/include)

# lis
file(GLOB_RECURSE LIS_SRC "src/lis/*")
add_executable(lis ${LIS_SRC})
target_link_libraries(lis compile_flags)

# team
file(GLOB_RECURSE TEAM_SRC "src/team/*")
add_executable(team ${TEAM_SRC})
target_link_libraries(team compile_flags)
#target_compile_definitions(team PUBLIC PARCEL_WRAPPING_POPS)
