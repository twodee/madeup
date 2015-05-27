cmake_minimum_required(VERSION 2.6)
project(madeup)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_subdirectory(src/twodee)
add_subdirectory(src/madeup)
add_subdirectory(src)
