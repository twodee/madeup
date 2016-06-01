set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_PREFIX_PATH "/Users/johnch/tarballs/Qt/5.6/clang_64")
find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)

# qt5_wrap_ui(UI_HEADERS mupper_window.ui)

add_executable(mupper
  mupper.cpp
  MupperWindow.cpp
  MadeupCanvas.cpp
  MadeupRenderer.cpp
  Log.cpp
  ShaderProgram.cpp
  Texture.cpp
  Trackball.cpp
  UniformManager.cpp
  Utilities.cpp
  UtilitiesOpenGL.cpp
  VertexArray.cpp
  VertexAttributes.cpp)
  # ${UI_HEADERS})
include_directories(mupper ${CMAKE_SOURCE_DIR}/include ${Qt5Widgets_INCLUDE_DIRS})
target_link_libraries(mupper madeup twodee Qt5::Widgets Qt5::OpenGL ${Qt5Gui_OPENGL_LIBRARIES})
set_target_properties(mupper PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set_target_properties(mupper PROPERTIES MACOSX_BUNDLE TRUE)

if(USE_IGL)
  set(IGL_INCLUDES
    ${CMAKE_BINARY_DIR}/libigl/include
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

  include_directories(mupper ${IGL_INCLUDES})
  target_link_libraries(mupper ${IGL_LIBRARIES})
endif()
