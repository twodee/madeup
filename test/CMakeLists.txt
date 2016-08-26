configure_file(test_mups ${CMAKE_BINARY_DIR}/test_mups @ONLY)

set(GTEST_ROOT "" CACHE PATH "The installation directory for Google Test")
find_package(GTest)

if (GTEST_FOUND)
  include_directories(${GTEST_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/include)
  add_executable(gtester
    main.cpp
    camera.cpp
    nfield.cpp
    nfielditerator.cpp
    qmath.cpp
    qmatrix.cpp
    qmatrix4.cpp
    qsquarematrix.cpp
    qvector.cpp
    qvector3.cpp
    scanner.cpp
  )
  target_link_libraries(gtester ${GTEST_BOTH_LIBRARIES} pthread madeup twodee)
endif(GTEST_FOUND)
