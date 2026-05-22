cmake_minimum_required(VERSION 3.15)

# ==========================================================
# 🧩 Project details
# ==========================================================
project(
  CHESS
  VERSION 0.1.0
  LANGUAGES C
)

message(STATUS "🚀 Starting configuration for ${PROJECT_NAME} v${PROJECT_VERSION}")

# ==========================================================
# ⚙️  Load custom project settings
# ==========================================================
include(cmake/StandardSettings.cmake)
include(cmake/Utils.cmake)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

# Define a cross-platform debug macro
add_compile_definitions($<$<CONFIG:Debug>:_DEBUG>)

# ==========================================================
# 🚫 Prevent in-source builds
# ==========================================================
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(FATAL_ERROR "❌ In-source builds are not allowed! Please use a separate build directory.")
endif()

# ==========================================================
# 📦 Package Managers (Vcpkg)
# ==========================================================
include(cmake/Vcpkg.cmake)

# ==========================================================
# 📁 Source & Header Files
# ==========================================================
include(cmake/SourcesAndHeaders.cmake)
message(STATUS "📂 Found project sources and headers")

# ==========================================================
# 🧱 Build Mode Logic
# ==========================================================
if(${PROJECT_NAME}_BUILD_EXECUTABLE)
  message(STATUS "🏗️ Building as EXECUTABLE")
  add_executable(${PROJECT_NAME} ${exe_sources})
  set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE C)

  if(${PROJECT_NAME}_ENABLE_UNIT_TESTING)
    message(STATUS "📚 Building test library: ${PROJECT_NAME}_LIB")
    add_library(${PROJECT_NAME}_LIB ${headers} ${sources})
  endif()

elseif(${PROJECT_NAME}_BUILD_HEADERS_ONLY)
  message(STATUS "📜 Building as HEADER-ONLY library")
  add_library(${PROJECT_NAME} INTERFACE)

else()
  message(STATUS "📦 Building as STATIC library (.a)")
  add_library(${PROJECT_NAME} ${headers} ${sources})
endif()

# Verbose output
if(${PROJECT_NAME}_VERBOSE_OUTPUT)
  if(${PROJECT_NAME}_BUILD_EXECUTABLE)
    verbose_message("Found the following sources:")
    foreach(source IN LISTS exe_sources)
      verbose_message("* ${source}")
    endforeach()
  elseif(NOT ${PROJECT_NAME}_BUILD_HEADERS_ONLY)
    verbose_message("Found the following sources:")
    foreach(source IN LISTS sources)
      verbose_message("* ${source}")
    endforeach()
  endif()

  verbose_message("Found the following headers:")
  foreach(header IN LISTS headers)
    verbose_message("* ${header}")
  endforeach()
endif()

# ==========================================================
# 🧾 Output directories
# ==========================================================
set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}"
)

if(${PROJECT_NAME}_BUILD_EXECUTABLE AND ${PROJECT_NAME}_ENABLE_UNIT_TESTING)
  set_target_properties(
    ${PROJECT_NAME}_LIB
    PROPERTIES
      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}"
      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}"
      OUTPUT_NAME ${PROJECT_NAME}
  )
endif()

# ==========================================================
# 🔗 Dependencies
# ==========================================================

# Vcpkg packages
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(cJSON CONFIG REQUIRED)

# Non-vcpkg external libraries
include(cmake/ExternalLib.cmake)

if(NOT ${PROJECT_NAME}_BUILD_HEADERS_ONLY)
  set(CHESS_LINK_LIBS
    ${EXTERNAL_LIBS}
    cjson
    $<TARGET_NAME_IF_EXISTS:sdl2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
    $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
  )

  target_link_libraries(${PROJECT_NAME} PRIVATE ${CHESS_LINK_LIBS})
  target_include_directories(${PROJECT_NAME} PUBLIC ${EXTERNAL_LIBS_INCLUDE})

  if(${PROJECT_NAME}_BUILD_EXECUTABLE AND ${PROJECT_NAME}_ENABLE_UNIT_TESTING)
    target_link_libraries(${PROJECT_NAME}_LIB PRIVATE ${CHESS_LINK_LIBS})
    target_include_directories(${PROJECT_NAME}_LIB PUBLIC ${EXTERNAL_LIBS_INCLUDE})
  endif()
endif()

# ==========================================================
# 📚 Include Directories
# ==========================================================
if(${PROJECT_NAME}_BUILD_HEADERS_ONLY)
  target_include_directories(
    ${PROJECT_NAME}
    INTERFACE
      $<INSTALL_INTERFACE:include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  )
else()
  target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
      $<INSTALL_INTERFACE:include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/src
  )

  if(${PROJECT_NAME}_BUILD_EXECUTABLE AND ${PROJECT_NAME}_ENABLE_UNIT_TESTING)
    target_include_directories(
      ${PROJECT_NAME}_LIB
      PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
  endif()
endif()

message(STATUS "✅ Include directories configured.")

# ==========================================================
# 📦 Installation Rules
# ==========================================================
install(
  TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(
  DIRECTORY include/${PROJECT_NAME_LOWERCASE}
  DESTINATION include
)

verbose_message("Install targets successfully built. Install with `cmake --build <build_directory> --target install --config <build_config>`.")

message(STATUS "📦 Installation setup complete.")

# ==========================================================
# 🧪 Unit Testing
# ==========================================================
if(${PROJECT_NAME}_ENABLE_UNIT_TESTING)
  enable_testing()
  message(STATUS "🧪 Unit testing ENABLED — looking for tests in /test/")
  add_subdirectory(test)
endif()

# ==========================================================
# 🧠 Build Summary
# ==========================================================
message(STATUS "")
message(STATUS "====== ${PROJECT_NAME} Configuration Summary ======")
message(STATUS "🔧 Build type:            ${CMAKE_BUILD_TYPE}")
message(STATUS "⚙️  Executable:            ${${PROJECT_NAME}_BUILD_EXECUTABLE}")
message(STATUS "📜 Headers-only:          ${${PROJECT_NAME}_BUILD_HEADERS_ONLY}")
message(STATUS "🧪 Unit testing enabled:  ${${PROJECT_NAME}_ENABLE_UNIT_TESTING}")
message(STATUS "📦 Install prefix:        ${CMAKE_INSTALL_PREFIX}")
message(STATUS "=====================================================")
message(STATUS "")
