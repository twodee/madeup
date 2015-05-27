add_executable(lex
  lex.cpp
)
include_directories(lex ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(lex madeup twodee)
