add_library(madeup
  SHARED
  Environment.cpp
  Expression.cpp
  ExpressionClosure.cpp
  ExpressionDefine.cpp
  ExpressionDefineVariable.cpp
  ExpressionDefineArrayElement.cpp
  Lexer.cpp
  Token.cpp
  SourceLocation.cpp
)
target_link_libraries(madeup twodee)
include_directories(madeup ${CMAKE_SOURCE_DIR}/include)
