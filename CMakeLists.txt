cmake_minimum_required(VERSION 2.6)
project(madeup)

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")

option(USE_DEBUG "Enable debug information" ON)

if(APPLE)
  set(WWW_ROOT "/Library/WebServer/Documents" CACHE STRING "Directory in which to install web resources")
elseif(UNIX)
  set(WWW_ROOT "/var/www/madeup" CACHE STRING "Directory in which to install web resources")
endif()
set(WWW_ROOT_NEEDS_SUDO TRUE CACHE BOOL "Is sudo needed to install to www root?")

if(USE_DEBUG AND CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  set(CMAKE_C_FLAGS "-pedantic -Wall -std=c99")
  set(CMAKE_CXX_FLAGS "-pedantic -Wall -std=c++11")

  if(USE_DEBUG)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
  endif()

  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -frounding-math")
  endif()
endif()

# Libigl imposes a bunch of dependencies that aren't fun to sort through. Let's
# allow for it to be disabled.
option(USE_IGL "Enable libigl" ON)
if(USE_IGL)
  add_definitions(-DUSE_IGL)

  find_package(Boost COMPONENTS log log_setup thread filesystem system)

  if(NOT DEFINED EIGEN3_INCLUDE_DIR)
    find_package(Eigen3 REQUIRED)
  endif()
  message(${EIGEN3_INCLUDE_DIR})

  find_package(CGAL REQUIRED)
  LIST(APPEND CMAKE_MODULE_PATH ${CGAL_DIR})
  find_package(GMP REQUIRED)
  find_package(MPFR REQUIRED)
endif()

add_subdirectory(src/twodee)
add_subdirectory(src/madeup)
add_subdirectory(src)
add_subdirectory(test)
option(BUILD_DESKTOP "Build desktop client" ON)
if(BUILD_DESKTOP)
  add_subdirectory(src/desktop)
endif()

if(WWW_ROOT_NEEDS_SUDO)
	set(SUDO "sudo")
else()
	set(SUDO "")
endif()

add_custom_target(www
  COMMAND make WWW_ROOT=${WWW_ROOT} SUDO=${SUDO} MADEUP=$<TARGET_FILE:madeup> TWODEE=$<TARGET_FILE:twodee> MERP=$<TARGET_FILE:merp>
  DEPENDS madeup twodee merp
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/www)

add_custom_target(wwwdeps
  COMMAND make WWW_ROOT=${WWW_ROOT} SUDO=${SUDO} MADEUP=$<TARGET_FILE:madeup> TWODEE=$<TARGET_FILE:twodee> MERP=$<TARGET_FILE:merp> libdeps
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/www)

add_custom_target(docs
  COMMAND make
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/docs)

add_custom_target(wwwdocs
  COMMAND make WWW_ROOT=${WWW_ROOT} wwwdocs
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/docs)
