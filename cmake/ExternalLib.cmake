set(EXTERNAL_LIBS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libs)

set(EXTERNAL_LIBS_INCLUDE
   ${EXTERNAL_LIBS_PATH}/fmod/include
   ${EXTERNAL_LIBS_PATH}/DataStructure/include
)

set(EXTERNAL_LIBS 
    ${EXTERNAL_LIBS_PATH}/fmod/lib/libfmod.so
    ${EXTERNAL_LIBS_PATH}/DataStructure/lib/libDATASTRUCTURE.a
)