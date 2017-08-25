include_directories(${CMAKE_SOURCE_DIR}/include ${TWODEE_INCLUDE_DIRECTORIES})

add_executable(lex lex.cpp)
target_link_libraries(lex madeup twodee pthread)
set_target_properties(lex PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(parse parse.cpp)
target_link_libraries(parse madeup twodee pthread ${IGL_LIBRARIES})
set_target_properties(parse PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(merp merp.cpp)
target_link_libraries(merp madeup twodee pthread ${IGL_LIBRARIES})
set_target_properties(merp PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if(USE_IGL)
  set(IGL_INCLUDES
    ${CMAKE_SOURCE_DIR}/libigl/include
    ${EIGEN3_INCLUDE_DIR}
    /usr/local/include
  )

  include_directories(${IGL_INCLUDES})

  set(IGL_LIBRARIES 
    ${Boost_LOG_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}

    ${Boost_LOG_SETUP_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}

    ${CGAL_LIBRARY}
    ${GMP_LIBRARIES}
    ${MPFR_LIBRARIES}
  )

  target_link_libraries(parse ${IGL_LIBRARIES})
  target_link_libraries(merp ${IGL_LIBRARIES})
endif()
