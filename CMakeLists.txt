cmake_minimum_required(VERSION 3.16)
project(stl-to-eznec VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Cross-platform configuration
if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# Find required packages
find_package(PkgConfig REQUIRED)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/main.cpp
    src/stl_parser.cpp
    src/material_database.cpp
    src/antenna_detector.cpp
    src/frequency_calculator.cpp
    src/nec_generator.cpp
    src/ez_generator.cpp
    src/user_interface.cpp
    src/geometry_utils.cpp
)

# Header files
set(HEADERS
    include/stl_parser.h
    include/material_database.h
    include/antenna_detector.h
    include/frequency_calculator.h
    include/nec_generator.h
    include/ez_generator.h
    include/user_interface.h
    include/geometry_utils.h
)

# Create executable
add_executable(stl-to-eznec ${SOURCES} ${HEADERS})

# Compiler-specific options
if(MSVC)
    target_compile_options(stl-to-eznec PRIVATE /W4)
else()
    target_compile_options(stl-to-eznec PRIVATE -Wall -Wextra -pedantic)
endif()

# Installation
install(TARGETS stl-to-eznec DESTINATION bin)
