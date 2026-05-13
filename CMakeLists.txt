cmake_minimum_required(VERSION 3.16)

project(
    HaloCyberAccessEngine
    VERSION 1.0.0
    DESCRIPTION "Cyber Access Engine for Security Log Analysis"
    LANGUAGES CXX
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set default build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Add compiler flags
add_compile_options(-Wall -Wextra)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src/include)

# Glob all source files from src/source/ and subdirectories
file(GLOB_RECURSE SOURCES "src/source/*.cpp")

# Main executable
add_executable(halo src/main.cpp ${SOURCES})

# Set output directories based on build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_target_properties(halo PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/debug/bin"
    )
else()
    set_target_properties(halo PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/release/bin"
    )
endif()

# Print build information
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Source files found: ${SOURCES}")
message(STATUS "Executable target: halo")
