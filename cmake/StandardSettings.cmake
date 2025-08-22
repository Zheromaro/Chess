#
# Project settings
#

option(${PROJECT_NAME}_BUILD_EXECUTABLE "Build the project as an executable, rather than a library." ON)
option(${PROJECT_NAME}_BUILD_HEADERS_ONLY "Build the project as a header-only library." OFF)
option(${PROJECT_NAME}_USE_ALT_NAMES "Use alternative names for the project, such as naming the include directory all lowercase." ON)

#
# Compiler options
#

option(${PROJECT_NAME}_WARNINGS_AS_ERRORS "Treat compiler warnings as errors." OFF)

#
# Package managers
#
# Currently supporting: Vcpkg.

option(${PROJECT_NAME}_ENABLE_VCPKG "Enable the Vcpkg package manager for this project." ON)

#
# Unit testing
#
# Currently supporting: GoogleTest.

option(${PROJECT_NAME}_ENABLE_UNIT_TESTING "Enable unit tests for the projects (from the `test` subfolder)." ON)

option(${PROJECT_NAME}_USE_GTEST "Use the GoogleTest project for creating unit tests." ON)
option(${PROJECT_NAME}_USE_GOOGLE_MOCK "Use the GoogleMock project for extending the unit tests." OFF)
