cmake_minimum_required(VERSION 2.6)
project(monitor)

# -----------------------------------------------------------------------------
# Curses Setup
# -----------------------------------------------------------------------------
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

# -----------------------------------------------------------------------------
# Project Source Files
# -----------------------------------------------------------------------------
include_directories(include)
file(GLOB SOURCES "src/*.cpp")

# Define main executable target
add_executable(monitor ${SOURCES})
target_link_libraries(monitor ${CURSES_LIBRARIES})
target_compile_options(monitor PRIVATE -Wall -Wextra)
set_property(TARGET monitor PROPERTY CXX_STANDARD 17)

# -----------------------------------------------------------------------------
# Google Test Setup
# -----------------------------------------------------------------------------
# Find Google Test libraries from common locations
find_library(GTEST_LIB gtest 
    HINTS /usr/lib/x86_64-linux-gnu /usr/src/googletest/googletest/lib
)
find_library(GTEST_MAIN_LIB gtest_main 
    HINTS /usr/lib/x86_64-linux-gnu /usr/src/googletest/googletest/lib
)

# Include Google Test headers
include_directories(/usr/include /usr/src/googletest/googletest/include)

# -----------------------------------------------------------------------------
# Test Executable
# -----------------------------------------------------------------------------
add_executable(
    monitor_test 
    test/test.cpp 
    src/linux_parser.cpp
)
target_link_libraries(monitor_test ${GTEST_LIB} ${GTEST_MAIN_LIB} pthread)

# -----------------------------------------------------------------------------
# Additional Notes
# -----------------------------------------------------------------------------
# TODO: In CI, add the -Werror flag to treat warnings as errors.
