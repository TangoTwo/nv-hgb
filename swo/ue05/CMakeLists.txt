cmake_minimum_required(VERSION 3.12)
project(ue05 C)

set(CMAKE_C_STANDARD 11)

add_library(compile_flags INTERFACE)
target_compile_options(compile_flags INTERFACE -Wall -Wconversion -Werror=vla -Wextra -Wpedantic)

include_directories(./src/include)

# adt
file(GLOB_RECURSE ADT_SRC "src/adt/*")
##list
add_executable(adtl ${ADT_SRC})
target_link_libraries(adtl compile_flags)
target_compile_definitions(adtl PUBLIC UE05_USE_LIST)
##matrix
add_executable(adtm ${ADT_SRC})
target_link_libraries(adtm compile_flags)

# this is needed for top so that
# the implementation files can use the
# original headers
include_directories(./src/adt)
# top
file(GLOB_RECURSE TOP_SRC "src/top/*")
set(TOP_SRC ${TOP_SRC}
        "src/adt/directed_graph.c"
        "src/adt/directed_graph_list.c"
        "src/adt/directed_graph_mat.c"
        "src/adt/node_list.c")
add_executable(top ${TOP_SRC})
target_link_libraries(top compile_flags)
