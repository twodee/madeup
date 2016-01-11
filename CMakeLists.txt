cmake_minimum_required(VERSION 2.6)
project(madeup)

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")

option(USE_DEBUG "Enable debug information" ON)

if(APPLE)
  set(WWW_ROOT "/Library/WebServer/Documents" CACHE STRING "Directory in which to install web resources")
elseif(UNIX)
  set(WWW_ROOT "/var/www/madeup" CACHE STRING "Directory in which to install web resources")
endif()

# Put all the executables and libs at the top level for easy access.
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}) 
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}) 

if(USE_DEBUG AND CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  set(CMAKE_C_FLAGS "-g -pedantic -Wall -std=c99")
  set(CMAKE_CXX_FLAGS "-g -pedantic -Wall -std=c++0x")
endif()

add_subdirectory(src/twodee)
add_subdirectory(src/madeup)
add_subdirectory(src)
add_subdirectory(test)

add_custom_target(www
  COMMAND make WWW_ROOT=${WWW_ROOT} MADEUP=$<TARGET_FILE:madeup> TWODEE=$<TARGET_FILE:twodee> MERP=$<TARGET_FILE:merp>
  DEPENDS madeup twodee merp
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/www)

add_custom_target(wwwdeps
  COMMAND make WWW_ROOT=${WWW_ROOT} MADEUP=$<TARGET_FILE:madeup> TWODEE=$<TARGET_FILE:twodee> MERP=$<TARGET_FILE:merp> libdeps
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/www)

add_custom_target(docs
  COMMAND make
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/docs)

add_custom_target(wwwdocs
  COMMAND make WWW_ROOT=${WWW_ROOT} wwwdocs
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/docs)
