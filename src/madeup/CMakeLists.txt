include_directories(${CMAKE_SOURCE_DIR}/include ${TWODEE_INCLUDE_DIRECTORIES})

set(LIBMADEUP_SRC_EXTRAS)
if(USE_IGL)
  list(APPEND LIBMADEUP_SRC_EXTRAS MeshBoolean.cpp)
endif()

include_directories(${TWODEE_INCLUDE_DIRECTORIES} ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/open-simplex-noise-in-c)

add_library(madeup
  STATIC
  ../../../open-simplex-noise-in-c/open-simplex-noise.c
  Environment.cpp
  Expression.cpp
  ExpressionAbsoluteValue.cpp
  ExpressionAdd.cpp
  ExpressionAll.cpp
  ExpressionAny.cpp
  ExpressionAnd.cpp
  ExpressionAppend.cpp
  ExpressionArray.cpp
  ExpressionBlobs.cpp
  ExpressionBlock.cpp
  ExpressionBoolean.cpp
  ExpressionBoxes.cpp
  ExpressionCall.cpp
  ExpressionCallWithNamedParameters.cpp
  ExpressionCamera.cpp
  ExpressionCenter.cpp
  ExpressionClosure.cpp
  ExpressionCoalesce.cpp
  ExpressionComment.cpp
  ExpressionCosine.cpp
  ExpressionCross.cpp
  ExpressionDebug.cpp
  ExpressionDefine.cpp
  ExpressionDefineElement.cpp
  ExpressionDefineVariable.cpp
  ExpressionDilate.cpp
  ExpressionDistort.cpp
  ExpressionDivide.cpp
  ExpressionDot.cpp
  ExpressionDowel.cpp
  ExpressionDynamicMinus.cpp
  ExpressionEcho.cpp
  ExpressionEqual.cpp
  ExpressionExtrude.cpp
  ExpressionFor.cpp
  ExpressionForget.cpp
  ExpressionFracture.cpp
  ExpressionGreater.cpp
  ExpressionGreaterOrEqual.cpp
  ExpressionHome.cpp
  ExpressionIdentifier.cpp
  ExpressionIdentity.cpp
  ExpressionIf.cpp
  ExpressionInteger.cpp
  ExpressionInverseCosine.cpp
  ExpressionInverseSine.cpp
  ExpressionInverseTangent.cpp
  ExpressionLesser.cpp
  ExpressionLesserOrEqual.cpp
  ExpressionLoft.cpp
  ExpressionLog.cpp
  ExpressionLook.cpp
  ExpressionMagnitude.cpp
  ExpressionMax.cpp
  ExpressionMesh.cpp 
  ExpressionMin.cpp
  ExpressionMirror.cpp
  ExpressionModeAdd.cpp
  ExpressionModeSubtract.cpp
  ExpressionMove.cpp
  ExpressionMoveX.cpp
  ExpressionMoveTo.cpp
  ExpressionMultiply.cpp
  ExpressionNegation.cpp
  ExpressionNetwork.cpp
  ExpressionNodes.cpp
  ExpressionNoise.cpp
  ExpressionNormalize.cpp
  ExpressionNot.cpp
  ExpressionNotEqual.cpp
  ExpressionNumber.cpp
  ExpressionOr.cpp
  ExpressionPath.cpp
  ExpressionPitch.cpp
  ExpressionPolarTo.cpp
  ExpressionPolygon.cpp
  ExpressionPop.cpp
  ExpressionPower.cpp
  ExpressionPrint.cpp
  ExpressionPush.cpp
  ExpressionRandom.cpp
  ExpressionRandom01.cpp
  ExpressionReal.cpp
  ExpressionRealDivide.cpp
  ExpressionReframe.cpp
  ExpressionRemainder.cpp
  ExpressionRepeat.cpp
  ExpressionRepeatwich.cpp
  ExpressionReturn.cpp
  ExpressionReverse.cpp
  ExpressionRevolve.cpp
  ExpressionRoll.cpp
  ExpressionRotate.cpp
  ExpressionScale.cpp
  ExpressionSign.cpp
  ExpressionSine.cpp
  ExpressionSize.cpp
  ExpressionSpheres.cpp
  ExpressionString.cpp
  ExpressionSubrange.cpp
  ExpressionSubscript.cpp
  ExpressionSubtract.cpp
  ExpressionSurface.cpp
  ExpressionTangent.cpp
  ExpressionTranslate.cpp
  ExpressionTransform.cpp
  ExpressionTrimesh.cpp
  ExpressionTube.cpp
  ExpressionUnit.cpp
  ExpressionUnknown.cpp
  ExpressionWhere.cpp
  ExpressionWhile.cpp
  ExpressionYaw.cpp
  Lexer.cpp
  Token.cpp
  SourceLocation.cpp
  Parser.cpp
  ${LIBMADEUP_SRC_EXTRAS}
)

if(USE_IGL)
  include_directories(
    madeup
    ${CMAKE_SOURCE_DIR}/libigl/include
    ${EIGEN3_INCLUDE_DIR}
    /usr/local/include
  )
  target_link_libraries(madeup pthread)
endif()
