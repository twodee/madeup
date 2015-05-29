cmake_minimum_required(VERSION 2.6)
project(madeup)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# if(CMAKE_COMPILER_IS_GNUCXX) 
  message("compiling with flags")
  set(CMAKE_C_FLAGS "-g -pedantic -Wall -std=c99")
  set(CMAKE_CXX_FLAGS "-g -pedantic -Wall -std=c++11")
# endif(CMAKE_COMPILER_IS_GNUCXX) 

add_subdirectory(src/twodee)
add_subdirectory(src/madeup)
add_subdirectory(src)
