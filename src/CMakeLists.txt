add_executable(lex lex.cpp)
include_directories(lex ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(lex madeup twodee)
set_target_properties(lex PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(parse parse.cpp)
include_directories(parse ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(parse madeup twodee)
set_target_properties(parse PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(merp merp.cpp)
include_directories(merp ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(merp madeup twodee)
set_target_properties(merp PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
