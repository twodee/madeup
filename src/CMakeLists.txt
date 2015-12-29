add_executable(lex lex.cpp)
include_directories(lex ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(lex madeup twodee)
set_target_properties(lex PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

find_package(Boost COMPONENTS log log_setup thread filesystem system)
add_executable(parse parse.cpp)
include_directories(parse
  ${CMAKE_SOURCE_DIR}/include
  /usr/local/include
  ${CMAKE_BINARY_DIR}/libigl/include
  /usr/local/include/eigen3
)
target_link_libraries(parse madeup twodee
  ${Boost_LOG_SETUP_LIBRARY}
  ${Boost_LOG_LIBRARY}
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_THREAD_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  madeup
  twodee
  /usr/local/lib/libmpfr.dylib
  /usr/local/lib/libgmp.dylib
  /usr/local/lib/libCGAL.dylib
  )
set_target_properties(parse PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(merp merp.cpp)
include_directories(merp
  ${CMAKE_SOURCE_DIR}/include
  /usr/local/include
  ${CMAKE_BINARY_DIR}/libigl/include
  /usr/local/include/eigen3
)
target_link_libraries(merp
  ${Boost_LOG_SETUP_LIBRARY}
  ${Boost_LOG_LIBRARY}
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_THREAD_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  madeup
  twodee
  /usr/local/lib/libmpfr.dylib
  /usr/local/lib/libgmp.dylib
  /usr/local/lib/libCGAL.dylib
)
set_target_properties(merp PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
