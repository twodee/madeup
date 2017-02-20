if(USE_IGL)
  add_executable(test_subtract test_subtract.cpp)

  include_directories(
    test_subtract
    ${CMAKE_SOURCE_DIR}/libigl/include
    ${EIGEN3_INCLUDE_DIR}
    /usr/local/include
  )

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

  target_link_libraries(test_subtract pthread ${IGL_LIBRARIES})
endif()

configure_file(test_mups ${CMAKE_BINARY_DIR}/test_mups @ONLY)
