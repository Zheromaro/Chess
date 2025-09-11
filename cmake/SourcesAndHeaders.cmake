file(GLOB_RECURSE sources 
    ${PROJECT_SOURCE_DIR}/src/GameLogic/*.c
    ${PROJECT_SOURCE_DIR}/src/LoopLogic/*.c
    ${PROJECT_SOURCE_DIR}/src/States/*.c
    ${PROJECT_SOURCE_DIR}/src/jsonReading.c
)

set(exe_sources
		src/main.c
		${sources}
)

file(GLOB_RECURSE headers 
    ${PROJECT_SOURCE_DIR}/include/*.h
    ${PROJECT_SOURCE_DIR}/include/LoopLogic/*.h
    ${PROJECT_SOURCE_DIR}/include/GameLogic/*.h
)

file(GLOB_RECURSE test_sources 
    ${PROJECT_SOURCE_DIR}/test/GameLogic/*.cpp
)

# test_dirs
set(TEST_DIR "${PROJECT_SOURCE_DIR}/test")

# Get all entries (files + dirs) recursively
file(GLOB_RECURSE ALL_PATHS
    LIST_DIRECTORIES true
    RELATIVE ${TEST_DIR}
    ${TEST_DIR}/*
)

# Keep only directories
set(test_dirs "")
foreach(entry ${ALL_PATHS})
    if(IS_DIRECTORY "${TEST_DIR}/${entry}")
        list(APPEND test_dirs "${entry}")
    endif()
endforeach()