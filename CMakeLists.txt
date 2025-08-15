cmake_minimum_required(VERSION 3.10)
project(CHESS VERSION 1.0)

# === Source Files ===
file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.c)

# === Setup External Static Library ===
set(FMOD_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libs/fmod)
set(FMOD_INCLUDE_DIR ${FMOD_PATH}/include)
set(FMOD_LIBRARY ${FMOD_PATH}/lib/libfmod.so)
set(DATA_STRUCTURE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libs/DataStructure)
set(DATA_STRUCTURE_INCLUDE_DIR ${DATA_STRUCTURE_PATH}/include)
set(DATA_STRUCTURE_LIBRARY ${DATA_STRUCTURE_PATH}/lib/libDATASTRUCTURE.a)

# === SDL2 and Related Packages ===
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)

# === Asset Copy ===
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/assets"
     DESTINATION "${CMAKE_BINARY_DIR}")

# === Executable ===
add_executable(${PROJECT_NAME} ${SRC_FILES})

# === Include Directories ===
target_include_directories(${PROJECT_NAME}
    PUBLIC 
        ${FMOD_INCLUDE_DIR}
        ${DATA_STRUCTURE_INCLUDE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# === Link External Static Library ===
target_link_libraries(${PROJECT_NAME}
    PRIVATE 
        ${FMOD_LIBRARY}
        ${DATA_STRUCTURE_LIBRARY}
)

# === SDL2 Linking ===
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        $<TARGET_NAME_IF_EXISTS:sdl2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
        $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
        $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
)
