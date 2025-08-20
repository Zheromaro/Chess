file(GLOB_RECURSE sources 
    ${PROJECT_SOURCE_DIR}/src/GameLogic/*.c
    ${PROJECT_SOURCE_DIR}/src/LoopLogic/*.c
    ${PROJECT_SOURCE_DIR}/src/States/*.c
)

set(exe_sources
		src/main.c
		${sources}
)

file(GLOB_RECURSE headers 
    ${PROJECT_SOURCE_DIR}/include/LoopLogic/*.h
    ${PROJECT_SOURCE_DIR}/include/GameLogic/*.h
)

set(test_sources
  tmp_test.cpp
)
