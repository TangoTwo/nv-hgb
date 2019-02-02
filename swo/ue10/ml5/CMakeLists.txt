cmake_minimum_required(VERSION 3.12)

project(ml5)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/static)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(compile_flags INTERFACE)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
   target_compile_options(compile_flags INTERFACE -Wall -Wextra -Wpedantic -Wconversion)

elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
   target_compile_definitions(compile_flags INTERFACE _CRT_SECURE_NO_WARNINGS)
   target_compile_options(compile_flags INTERFACE /W4 /MP)
endif()

find_package(glfw3  REQUIRED)
find_package(OpenGL REQUIRED)
find_package(wxWidgets COMPONENTS gl core base REQUIRED)

include(${wxWidgets_USE_FILE})

if(WIN32)
   string(REPLACE "-mwindows" "-mconsole" wxWidgets_LIBRARIES "${wxWidgets_LIBRARIES}")
   string(REPLACE "--subsystem,windows" "--subsystem,console" wxWidgets_LIBRARIES "${wxWidgets_LIBRARIES}")
endif()

file(GLOB_RECURSE MINILIB_SRC "src/*")
add_library(minilib SHARED ${MINILIB_SRC})
target_include_directories(minilib PUBLIC "inc" ${wxWidgets_INCLUDE_DIRS})
target_link_libraries(minilib PUBLIC compile_flags glfw OpenGL::GL OpenGL::GLU ${wxWidgets_LIBRARIES})
