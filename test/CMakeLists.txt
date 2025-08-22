cmake_minimum_required(VERSION 3.15)

#
# Project details
#

project(
  ${CMAKE_PROJECT_NAME}Tests
  LANGUAGES CXX
)

verbose_message("Adding tests under ${CMAKE_PROJECT_NAME}Tests...")

foreach(dir ${test_dirs})
  #
  # Get all sources in this directory
  #

  file(GLOB dir_sources ${dir}/*.cpp)

  #
  # executable
  #

  get_filename_component(dir_name ${dir} NAME)
  add_executable(${dir_name}_Test ${dir_sources})

  #
  # Set the compiler standard
  #

  target_compile_features(${dir_name}_Test PUBLIC cxx_std_17)

  #
  # Load the desired unit testing framework
  #
  # Currently supported: GoogleTest (and GoogleMock)

  if(${CMAKE_PROJECT_NAME}_BUILD_EXECUTABLE)
    set(${CMAKE_PROJECT_NAME}_TEST_LIB ${CMAKE_PROJECT_NAME}_LIB)
  else()
    set(${CMAKE_PROJECT_NAME}_TEST_LIB ${CMAKE_PROJECT_NAME})
  endif()

  if(${CMAKE_PROJECT_NAME}_USE_GTEST)
    find_package(GTest REQUIRED)

    if(${CMAKE_PROJECT_NAME}_USE_GOOGLE_MOCK)
      set(GOOGLE_MOCK_LIBRARIES GTest::gmock GTest::gmock_main)
    endif()

    target_link_libraries(
      ${dir_name}_Test
      PUBLIC
        GTest::GTest
        GTest::Main
        ${GOOGLE_MOCK_LIBRARIES}
        ${${CMAKE_PROJECT_NAME}_TEST_LIB}
    )
  else()
    message(FATAL_ERROR "Unknown testing library. Please setup your desired unit testing library by using `target_link_libraries`.")  
  endif()

  #
  # Add the unit tests to CTest
  #

  add_test(
    NAME
      ${dir_name}
    COMMAND
      ${dir_name}_Test
  )
endforeach()

verbose_message("Finished adding unit tests for ${CMAKE_PROJECT_NAME}.")
