set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# For ease of development, I'm guessing a shortcut to Qt is installed.
set(CMAKE_PREFIX_PATH "$ENV{HOME}/tarballs/qt")

find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)

set(MADEUP_EXECUTABLE "Madeup")
add_executable(${MADEUP_EXECUTABLE}
  madeup.cpp
  MadeupWindow.cpp
  MadeupCanvas.cpp
  MadeupRenderer.cpp
  Log.cpp
  ShaderProgram.cpp
  Texture.cpp
  Trackball.cpp
  UniformManager.cpp
  UtilitiesOpenGL.cpp
  VertexArray.cpp
  VertexAttributes.cpp
  jsoncpp.cpp)
include_directories(${MADEUP_EXECUTABLE} ${CMAKE_SOURCE_DIR}/include ${Qt5Widgets_INCLUDE_DIRS})
target_link_libraries(${MADEUP_EXECUTABLE} madeup twodee Qt5::Widgets Qt5::OpenGL GL)
set_target_properties(${MADEUP_EXECUTABLE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set_target_properties(${MADEUP_EXECUTABLE} PROPERTIES MACOSX_BUNDLE TRUE)

if(NOT APPLE)
  set_target_properties(${MADEUP_EXECUTABLE} PROPERTIES OUTPUT_NAME madeup)
endif()

if(USE_IGL)
  set(IGL_INCLUDES
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

  include_directories(${MADEUP_EXECUTABLE} ${IGL_INCLUDES})
  target_link_libraries(${MADEUP_EXECUTABLE} ${IGL_LIBRARIES})
endif()

if(USE_MAGICK)
  target_link_libraries(${MADEUP_EXECUTABLE} ${ImageMagick_LIBRARIES})
endif()
