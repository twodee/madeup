add_executable(lex lex.cpp)
include_directories(lex ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(lex madeup twodee)

add_executable(parse parse.cpp)
include_directories(parse ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(parse madeup twodee)

add_executable(merp merp.cpp)
include_directories(merp ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(merp madeup twodee)
