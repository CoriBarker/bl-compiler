cmake_minimum_required(VERSION 3.10)

# Set the project name and language
project(blc CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the include directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# Gather all source files from the src directory
file(GLOB SOURCES "src/**/*.cpp", "src/*.cpp")
message(STATUS "Sources: ${SOURCES}")

# Define the executable
add_executable(blc ${SOURCES})
