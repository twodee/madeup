add_library(twodee
  STATIC
  Camera.cpp
  FieldTraits.cpp
  Log.cpp
  MessagedException.cpp
  NFieldBase.cpp
  Scanner.cpp
  Trimesh.cpp
  Utilities.cpp
)
include_directories(twodee ${CMAKE_SOURCE_DIR}/include)
